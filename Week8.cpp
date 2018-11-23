// Week8.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <functional>
#include <stdio.h>
#include <sstream>
using namespace tle;

/* ################### ASK THIS #########################

void Forward_Backwards_Movement(float F_SteerAngle, float F_Speed, float F_RotationSpeed)
{
Matchbox->RotateLocalY(F_SteerAngle);

Matchbox->MoveLocalZ(F_Speed);
MatchFront->RotateLocalX(F_RotationSpeed * 3);
MatchRear->RotateLocalX(F_RotationSpeed * 3);

return;
}

	################### ASK THIS ######################### */

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartFullscreen(1920, 1080);

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	// camera 
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 80, -160);
	camera->RotateX(25);

	// meshs
	IMesh* MatchboxMesh = myEngine->LoadMesh("Matchbox.x");
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IMesh* MatchMesh = myEngine->LoadMesh("Match.x");
	IMesh* TwoPenceMesh = myEngine->LoadMesh("TwoPence.x");
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");

	// enviroment models
	IModel* Floor = FloorMesh->CreateModel(0, 0, 0);
	IModel* Grid = GridMesh->CreateModel(0, 0.1, 0);

	// matchbox car models
	IModel* Matchbox = MatchboxMesh->CreateModel(0, 13, 0);
	IModel* MatchFront = MatchMesh->CreateModel();
	IModel* MatchRear = MatchMesh->CreateModel();
	IModel* TwoPenceFrontR = TwoPenceMesh->CreateModel();
	IModel* TwoPenceRearR = TwoPenceMesh->CreateModel();
	IModel* TwoPenceFrontL = TwoPenceMesh->CreateModel();
	IModel* TwoPenceRearL = TwoPenceMesh->CreateModel();

	// matchbox car dimentions
	const float MatchboxWidth = 18.0f;
	const float MatchboxLenght = 25.0f;
	const float AxlesOffset = 2.0f;
	const float TyresOffset = 4.0f;

	// matchbox car parts attach
	MatchFront->AttachToParent(Matchbox);
	MatchRear->AttachToParent(Matchbox);
	TwoPenceFrontR->AttachToParent(MatchFront);
	TwoPenceFrontL->AttachToParent(MatchFront);
	TwoPenceRearR->AttachToParent(MatchRear);
	TwoPenceRearL->AttachToParent(MatchRear);

	// Positioning Matchbox car parts
	MatchFront->MoveZ(MatchboxLenght - AxlesOffset);
	MatchRear->MoveZ(-MatchboxLenght + AxlesOffset);
	TwoPenceFrontR->MoveX(MatchboxWidth + TyresOffset);
	TwoPenceFrontL->MoveX(-MatchboxWidth - TyresOffset);
	TwoPenceRearR->MoveX(-MatchboxWidth - TyresOffset);
	TwoPenceRearL->MoveX(MatchboxWidth + TyresOffset);

	camera->AttachToParent(Matchbox);

	// MatchBox car variables
	/*
	const float MaxSpeed = 1.5f;
	const float SpeedAcceleration = 0.01f;
	float CurrentSpeed = 0.0f;
	float RotationSpeed = 0.2f;
	float RotationLimit = 25.0f;
	float MatchFrontRot = 0.0f;
	float SteerAngle = 0.0f;
	float SteerAngleIncrease = 0.005f;

	float SteerAngleRotSpeed = CurrentSpeed;
	*/

	// New MatchBox car variables
	const float maxSpeed = 2.0f;
	const float minSpeed = 0.0f;
	float CurrentSpeed = 0.0f;
	float SpeedAcceleration = 0.005f;
	bool PressingW = false;

	// text test
	IFont* Font_CurrentSpeed = myEngine->LoadFont("Comic Sans MS", 40);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Steering Angle Text
		stringstream S_CurrentSpeed;
		S_CurrentSpeed << "SteerAngle: " << CurrentSpeed;
		Font_CurrentSpeed->Draw(S_CurrentSpeed.str(), 30, 1025);

		// New Car movement
		Matchbox->MoveLocalZ(CurrentSpeed);

		if (myEngine->KeyHeld(Key_W) && CurrentSpeed<=maxSpeed)
		{
			CurrentSpeed = CurrentSpeed + SpeedAcceleration;
			PressingW = true;
		}
		else {
			PressingW = false;
		}

		if (PressingW == false && CurrentSpeed>=minSpeed)
		{
			CurrentSpeed = CurrentSpeed - SpeedAcceleration;

			if (CurrentSpeed<=minSpeed)
			{
				PressingW = true;
			}
		}

		// car movement
		/*
		// front movement
		if (myEngine->KeyHeld(Key_Up))
		{
			//Forward_Backwards_Movement(SteerAngle, Speed, RotationSpeed);

			if (CurrentSpeed <= MaxSpeed)
			{
				CurrentSpeed = CurrentSpeed + SpeedAcceleration;
				SteerAngle = SteerAngle + SteerAngleIncrease;
			}

			Matchbox->RotateLocalY(SteerAngle);

			Matchbox->MoveLocalZ(CurrentSpeed);
			MatchFront->RotateLocalX(RotationSpeed * 3);
			MatchRear->RotateLocalX(RotationSpeed * 3);
			
		}

		// back movement
		
		if (myEngine->KeyHeld(Key_Down))
		{
			Matchbox->RotateLocalY(-SteerAngle);

			Matchbox->MoveLocalZ(-CurrentSpeed);
			MatchFront->RotateLocalX(-RotationSpeed * 3);
			MatchRear->RotateLocalX(-RotationSpeed * 3);
		}
		

		// Steer Right
		if (myEngine->KeyHeld(Key_Right))
		{
			if (MatchFrontRot <= RotationLimit)
			{
				MatchFrontRot = MatchFrontRot + RotationSpeed;
				MatchFront->RotateY(RotationSpeed);

				//SteerAngle = SteerAngle + SteerAngleRotSpeed;
			}
		}

		// Steer Left
		if (myEngine->KeyHeld(Key_Left))
		{
			if (MatchFrontRot >= -RotationLimit)
			{
				MatchFrontRot = MatchFrontRot - RotationSpeed;
				MatchFront->RotateY(-RotationSpeed);

				SteerAngle = SteerAngle -SteerAngleRotSpeed;
			}
		}
		*/

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
