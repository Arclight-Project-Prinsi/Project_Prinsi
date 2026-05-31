// Fill out your copyright notice in the Description page of Project Settings.


#include "Prinsi/Entity/Player/BasePlayerCharacter.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ~~初始化组件
	EntityComp_ = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));	// Entity组件
	SpringArmComp_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	TopDownCamera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	// ~摄像机与弹簧臂
	{
		SpringArmComp_->SetupAttachment(RootComponent);
		SpringArmComp_->TargetArmLength = 1400.0f;
		SpringArmComp_->SetRelativeRotation(FRotator(-50.0f, 45.0f, 0.0f));
		SpringArmComp_->bUsePawnControlRotation = false;

		TopDownCamera_->SetupAttachment(SpringArmComp_);
		TopDownCamera_->bUsePawnControlRotation = false;
		TopDownCamera_->SetFieldOfView(55.0f);			// 视场角(TopDown不需要广角)
	}
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	// ~~通过EntityId进行初始化(主表+扩展表)
	if (!EntityComp_) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件为空(BasePlayerCharacter.cpp)")); return;
	}
	if (!InitFromConfig(EntityComp_->GetEntityId())) {
		UE_LOG(LogTemp, Warning, TEXT("通过EntityId进行初始化失败(BasePlayerCharacter.cpp)")); return;
	}
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABasePlayerCharacter::InitFromConfig(FName Id) {
	// ~~读取主表
	if (!EntityBaseTable_) {
		UE_LOG(LogTemp, Warning, TEXT("EntityBaseTable为空(BasePlayerCharacter.cpp)"));
		return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityBaseConfig* BaseConfig =
		EntityBaseTable_->FindRow<FEntityBaseConfig>(Id, TEXT("无法找到对应Raw,(BasePlayerCharacter.cpp)"));
	if (!BaseConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应EntityBase配置数据, EntityId = %s(BasePlayerCharacter.cpp)"), *Id.ToString()); return false;
	}

	// ~~确认Entity种类正确
	if (BaseConfig->EntityType != EEntityType::EntityPlayer) {
		UE_LOG(LogTemp, Warning, TEXT(" EntityType不匹配,EntityId = %s(BasePlayerCharacter.cpp)"), *Id.ToString()); return false;
	}

	// ~~Entity组件初始化
	if (!EntityComp_->InitFromConfig(BaseConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Entity组件初始化失败(BasePlayerCharacter.cpp)")); return false;
	}

	// ~~读取Player扩展表(Player)
	if (!PlayerExtraTable_) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerExtraTable为空(BasePlayerCharacter.cpp)")); return false;
	}

	// ~~通过EntityId找到对应行(Raw)
	const FEntityPlayerExtraConfig* PlayerConfig =
		PlayerExtraTable_->FindRow<FEntityPlayerExtraConfig>(Id, TEXT("找不到PlayerExtra!"));
	if (!PlayerConfig) {
		UE_LOG(LogTemp, Warning,
			TEXT("无法找到对应Player配置数据, EntityId = %s(BasePlayerCharacter.cpp)"), *Id.ToString()); return false;
	}

	// ~~Player字段初始化
	if (!InitPlayerFromConfig(PlayerConfig)) {
		UE_LOG(LogTemp, Warning, TEXT("Player扩展字段初始化失败(BasePlayerCharacter.cpp)")); return false;
	}

	return true;
}

bool ABasePlayerCharacter::InitPlayerFromConfig(const FEntityPlayerExtraConfig* PlayerConfig) {
	if (!PlayerConfig) { return false; }

	Health_ = PlayerConfig->Health;
	return true;
}

