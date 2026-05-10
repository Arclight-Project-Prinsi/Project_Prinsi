// Fill out your copyright notice in the Description page of Project Settings.
#include "Prinsi/PlayerController/ArclightController.h"
#include "EnhancedInputComponent.h"						// 输入增强组件


void AArclightController::BeginPlay() {
	Super::BeginPlay();

	// ~~绑定IMC至EnhancedInput子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);		// 从ULocalPlayer处找它的子系统，
	}
}

void AArclightController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AArclightController::SetupInputComponent() {
	Super::SetupInputComponent();

	// ~~IA响应绑定
	// IMC触发IA，InputComponent监听IA，Controller响应IA（对应函数）
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		// 移动玩家角色
		if (IA_MovePlayer) { EIC->BindAction(IA_MovePlayer, ETriggerEvent::Triggered, this, &AArclightController::OnMovePlayer); }
	}
}

void AArclightController::OnMovePlayer(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();			// 获得Controller绑定的Pawn
	if (!MyPawn) return;

	// 位移处理
	MyPawn->AddMovementInput(FVector::ForwardVector, Input.Y);
	MyPawn->AddMovementInput(FVector::RightVector, Input.X);
}
