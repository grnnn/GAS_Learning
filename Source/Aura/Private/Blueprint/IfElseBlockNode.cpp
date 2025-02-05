// MyIfElseNode.cpp
#include "Blueprint/IfElseBlockNode.h"

#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphUtilities.h"
#include "EditorCategoryUtils.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "K2Node_MultiGate"

class FKCHandler_IfElseBlockNode : public FNodeHandlingFunctor
{
public:
	FKCHandler_IfElseBlockNode(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		// Make sure that the input pin is connected and valid for this block
		FEdGraphPinType ExpectedPinType;
		ExpectedPinType.PinCategory = UEdGraphSchema_K2::PC_Exec;

		UEdGraphPin* ExecTriggeringPin = Context.FindRequiredPinByName(Node, UEdGraphSchema_K2::PN_Execute, EGPD_Input);
		if ((ExecTriggeringPin == nullptr) || !Context.ValidatePinType(ExecTriggeringPin, ExpectedPinType))
		{
			CompilerContext.MessageLog.Error(*LOCTEXT("NoValidExecutionPinForExecSeq_Error", "@@ must have a valid execution pin @@").ToString(), Node, ExecTriggeringPin);
			return;
		}
		else if (ExecTriggeringPin->LinkedTo.Num() == 0)
		{
			CompilerContext.MessageLog.Warning(*LOCTEXT("NodeNeverExecuted_Warning", "@@ will never be executed").ToString(), Node);
			return;
		}

		// find all valid pin pairs
		TArray<TPair<UEdGraphPin*, UEdGraphPin*>> PinPairs;
		int CurrentIndex = 0;
		for (auto Pin : Node->Pins)
		{
			if (Pin->Direction == EGPD_Input &&
				Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Boolean &&
				Pin->LinkedTo.Num() > 0 &&
				Pin->PinName.ToString().StartsWith(UIfElseBlockNode::GetConditionNameGivenIndex(CurrentIndex).ToString()))
			{
				UEdGraphPin* OutputPin = Context.FindRequiredPinByName(Node, UIfElseBlockNode::GetBranchNameGivenIndex(CurrentIndex), EGPD_Output);
				if (OutputPin && OutputPin->LinkedTo.Num() > 0)
				{
					PinPairs.Add({Pin, OutputPin});
					++CurrentIndex;
				}
			}
		}

		// Iterate through pin pairs until one is found that fulfills the condition
		for (int i = 0; i < PinPairs.Num(); i++)
		{
			if (PinPairs[i].Key->LinkedTo.Num() > 0 && PinPairs[i].Value->LinkedTo.Num() > 0)
			{
				UEdGraphPin* PinToTry = FEdGraphUtilities::GetNetFromPin(PinPairs[i].Key);
				FBPTerminal** CondTerm = Context.NetMap.Find(PinToTry);
				
				// If the condition is true, go to the branch
				if (CondTerm != nullptr)
				{
					FBlueprintCompiledStatement& GotoThen = Context.AppendStatementForNode(Node);
					GotoThen.Type = KCST_UnconditionalGoto;
					GotoThen.LHS = *CondTerm;
					Context.GotoFixupRequestMap.Add(&GotoThen, PinPairs[i].Value);
				}
			}
		}

		// If no conditions are met, go to the else branch
		UEdGraphPin* ElsePin = Context.FindRequiredPinByName(Node, FName(TEXT("Else")), EGPD_Output);
		FBlueprintCompiledStatement& GotoElse = Context.AppendStatementForNode(Node);
		GotoElse.Type = KCST_UnconditionalGoto;
		Context.GotoFixupRequestMap.Add(&GotoElse, ElsePin);
	}
};

void UIfElseBlockNode::AllocateDefaultPins()
{
	// input execution pin
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	// hidden output pin to line up conditions with branches
	UEdGraphPin* SpacerPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName(*FString::Printf(TEXT(" "))));
	//SpacerPin->bHidden = true;  // Hide the spacer pin
	SpacerPin->bNotConnectable = true;  // Make it impossible to connect to

	// condition and branch pin pairs
	for (int32 i = 0; i < BranchPinCount; ++i)
	{
		auto InputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetConditionNameGivenIndex(i));
		auto OutputPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetBranchNameGivenIndex(i));
		PinPairs.Add({InputPin, OutputPin});
	}

	// else case
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName(TEXT("Else")));

	Super::AllocateDefaultPins();
}

FText UIfElseBlockNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("If-Else Block"));
}

FText UIfElseBlockNode::GetTooltipText() const
{
	return FText::FromString(TEXT("A custom if-else block node with variable conditions and branches."));
}

void UIfElseBlockNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UIfElseBlockNode::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::FlowControl);
}

class FNodeHandlingFunctor* UIfElseBlockNode::CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_IfElseBlockNode(CompilerContext);
}

void UIfElseBlockNode::AddInputPin()
{
	++BranchPinCount;
	ReconstructNode();
}

void UIfElseBlockNode::InsertPinIntoExecutionNode(UEdGraphPin* PinToInsertBefore, EPinInsertPosition Position)
{
	Modify();

	int32 DesiredPinIndex = Pins.Find(PinToInsertBefore);
	if (DesiredPinIndex != INDEX_NONE)
	{
		if (Position == EPinInsertPosition::After)
		{
			DesiredPinIndex = DesiredPinIndex + 1;
		}

		FCreatePinParams Params;
		Params.Index = DesiredPinIndex;
		auto InputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FName(), Params);
		auto OutputPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName(), Params);
		PinPairs.Insert({InputPin, OutputPin}, DesiredPinIndex);

		// refresh names on the pin list:
		int32 InIndex = 0;
		int32 OutIndex = 0;
		for (int32 Idx = 0; Idx < Pins.Num(); ++Idx)
		{
			UEdGraphPin* PotentialPin = Pins[Idx];
			if (PotentialPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Boolean && (PotentialPin->Direction == EGPD_Input))
			{
				PotentialPin->PinName = GetConditionNameGivenIndex(InIndex);
				++InIndex;
			}
			if (UEdGraphSchema_K2::IsExecPin(*PotentialPin) && (PotentialPin->Direction == EGPD_Output))
			{
				PotentialPin->PinName = GetBranchNameGivenIndex(OutIndex);
				++OutIndex;
			}
		}
	}
}

void UIfElseBlockNode::RemovePinFromExecutionNode(UEdGraphPin* TargetPin)
{
	UIfElseBlockNode* OwningBlockNode = Cast<UIfElseBlockNode>( TargetPin->GetOwningNode() );
	if (OwningBlockNode)
	{
		OwningBlockNode->Pins.Remove(TargetPin);
		UEdGraphPin* PairedPin = nullptr;
		for (auto Pair : PinPairs)
		{
			if (TargetPin == Pair.Key)
			{
				PinPairs.Remove(Pair);
				PairedPin = Pair.Value;
				OwningBlockNode->Pins.Remove(PairedPin);
				break;
			}
			else if (TargetPin == Pair.Value)
			{
				PinPairs.Remove(Pair);
				PairedPin = Pair.Key;
				OwningBlockNode->Pins.Remove(PairedPin);
				break;
			}
		}
		TargetPin->MarkAsGarbage();
		PairedPin->MarkAsGarbage();

		// Renumber the pins so the numbering is compact
		int32 InIndex = 0;
		int32 OutIndex = 0;
		for (int32 i = 0; i < OwningBlockNode->Pins.Num(); ++i)
		{
			UEdGraphPin* PotentialPin = OwningBlockNode->Pins[i];
			if (PotentialPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Boolean && (PotentialPin->Direction == EGPD_Input))
			{
				PotentialPin->PinName = GetConditionNameGivenIndex(InIndex);
				++InIndex;
			}
			if (UEdGraphSchema_K2::IsExecPin(*PotentialPin) && (PotentialPin->Direction == EGPD_Output))
			{
				PotentialPin->PinName = GetBranchNameGivenIndex(OutIndex);
				++OutIndex;
			}
		}
	}
}

bool UIfElseBlockNode::CanRemoveExecutionPin() const
{
	int32 NumOutPins = 0;

	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		UEdGraphPin* PotentialPin = Pins[i];
		if (UEdGraphSchema_K2::IsExecPin(*PotentialPin) && (PotentialPin->Direction == EGPD_Output))
		{
			NumOutPins++;
		}
	}

	return (NumOutPins > 2);
}

UEdGraphPin* UIfElseBlockNode::GetThenPinGivenIndex(int32 Index)
{
	return FindPin(GetBranchNameGivenIndex(Index));
}

FName UIfElseBlockNode::GetBranchNameGivenIndex(int32 Index)
{
	return *FString::Printf(TEXT("Branch %d"), Index + 1);
}

FName UIfElseBlockNode::GetConditionNameGivenIndex(int32 Index)
{
	return *FString::Printf(TEXT("Condition %d"), Index + 1);
}
