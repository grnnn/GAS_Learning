// Copyright Taylor Owen-Milner
#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "IfElseBlockNode.generated.h"

UCLASS(meta = (Keywords = "if else"))
class AURA_API UIfElseBlockNode : public UK2Node, public IK2Node_AddPinInterface
{
	GENERATED_BODY()

public:
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;

	// UK2Node interface
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool CanEverInsertExecutionPin() const override { return true; }
	virtual bool CanEverRemoveExecutionPin() const override { return true; }

	// IK2Node_AddPinInterface interface
	virtual void AddInputPin() override;

	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> GetPinPairs() const { return PinPairs; }

	// The below functions are called directly in the editor in BlueprintEditor.cpp.
	//	They are not part of the IK2Node_AddPinInterface interface, but they obviously should be.
	//	Going to comment out the abstractness and cry about it later.
	/*virtual*/	void InsertPinIntoExecutionNode(UEdGraphPin* PinToInsertBefore, EPinInsertPosition Position) /*override*/;
	/*virtual*/ void RemovePinFromExecutionNode(UEdGraphPin* TargetPin) /*override*/;
	/*virtual*/ bool CanRemoveExecutionPin() const /*override*/;
	/*virtual*/ UEdGraphPin* GetThenPinGivenIndex(int32 Index) /*override*/;

	static FName GetBranchNameGivenIndex(int32 Index);
	static FName GetConditionNameGivenIndex(int32 Index);

private:
	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> PinPairs;
	int32 BranchPinCount = 1;
};