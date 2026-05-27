
#include "InteractableComponent.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

// Widget
#include "DorothysJob/UI/BaseWidget.h"
#include "DorothysJob/UI/World/InteractPromt.h"
#include "Components/WidgetComponent.h"

UInteractableComponent::UInteractableComponent()
{
  m_bIsOverlapping = false;
  m_pPlayer = nullptr;
  m_pInteractPromptWidget = nullptr;
}

void UInteractableComponent::OnRegister() {
  Super::OnRegister();
  
  if (!m_pInteractPromptWidget)
  {
    m_pInteractPromptWidget = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("InteractPrompt"));
    m_pInteractPromptWidget->SetupAttachment(this);
    m_pInteractPromptWidget->RegisterComponent();
  }

  if (m_oInteractPromptClass) {
    m_pInteractPromptWidget->SetWidgetClass(m_oInteractPromptClass);
    m_pInteractPromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
    m_pInteractPromptWidget->SetRelativeLocation(m_vRelativePosition);
  }
}

void UInteractableComponent::BeginPlay()
{
  Super::BeginPlay();

  OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnInteractableBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnInteractableEndOverlap);
}

void UInteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  OnComponentBeginOverlap.RemoveAll(this);
  OnComponentEndOverlap.RemoveAll(this);

  if (m_pPlayer)
  {
    m_pPlayer->OnInteract.RemoveAll(this);
  }
}


bool UInteractableComponent::IsOverlap() const
{
  return m_bIsOverlapping;
}


void UInteractableComponent::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (ABasePlayer* player = Cast<ABasePlayer>(OtherActor))
  {
    m_bIsOverlapping = true;
    m_pPlayer = player;

    m_pPlayer->OnInteract.AddUObject(this, &UInteractableComponent::Interact);

    if (UBaseWidget* pBaseWidget = Cast<UBaseWidget>(m_pInteractPromptWidget->GetUserWidgetObject())) {
      if (UInteractPromt* pInteractPrompt = Cast<UInteractPromt>(pBaseWidget)) {
        pInteractPrompt->Show();
      }
    }
  }
}

void UInteractableComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (Cast<AActor>(m_pPlayer) == OtherActor)
  {
    m_pPlayer->OnInteract.RemoveAll(this);

    m_bIsOverlapping = false;
    m_pPlayer = nullptr;

    if (UBaseWidget* pBaseWidget = Cast<UBaseWidget>(m_pInteractPromptWidget->GetUserWidgetObject())) {
      if (UInteractPromt* pInteractPrompt = Cast<UInteractPromt>(pBaseWidget)) {
        pInteractPrompt->Hide();
      }
    }
  }
}

void UInteractableComponent::Interact()
{
  UE_LOG(LogTemp, Warning, TEXT("Interactable Base"))
}
