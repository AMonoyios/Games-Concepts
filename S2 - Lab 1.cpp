// Week8.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <functional>
#include <stdio.h>
#include <sstream>
using namespace tle;

void MatchboxFront(IModel* F_Matchbox, IModel* F_MatchFront, IModel* F_MatchRear, float F_SteerAngle, float F_Speed) {
	F_Matchbox->RotateLocalY(F_SteerAngle);
	F_Matchbox->MoveLocalZ(F_Speed);
	F_MatchFront->RotateLocalX(F_Speed * 3);
	F_MatchRear->RotateLocalX(F_Speed * 3);
}

void MatchboxBack(IModel* F_Matchbox, IModel* F_MatchFront, IModel* F_MatchRear, float F_SteerAngle, float F_Speed) {
	F_Matchbox->RotateLocalY(-F_SteerAngle);
	F_Matchbox->MoveLocalZ(-F_Speed);
	F_MatchFront->RotateLocalX(-F_Speed * 3);
	F_MatchRear->RotateLocalX(F_Speed * 3);
}

float MatchboxRight(float F_MatchFrontRot, float F_RotationSpeed, IModel* F_MatchFront, float F_SteerAngle, float F_SteerAngleIncrease) {
	F_MatchFront->RotateY(F_RotationSpeed);
	F_SteerAngle = F_SteerAngle + F_SteerAngleIncrease;

	return F_SteerAngle;
}

float MatchboxLeft(float F_MatchFrontRot, float F_RotationSpeed, IModel* F_MatchFront, float F_SteerAngle, float F_SteerAngleIncrease) {
	F_MatchFront->RotateY(-F_RotationSpeed);
	F_SteerAngle = F_SteerAngle - F_SteerAngleIncrease;

	return F_SteerAngle;
}

bool TurretFiringZone(float F_TurretX, float F_TurretY, float F_TurretZ, IModel* F_Matchbox, float F_MatchboxX, float F_MatchboxY, float F_MatchboxZ, float F_CarCollisionArea) {
	
	// getting position of matchbox
	F_MatchboxX = F_Matchbox->GetX();
	F_MatchboxY = F_Matchbox->GetY();
	F_MatchboxZ = F_Matchbox->GetZ();

	float ex = F_TurretX - F_MatchboxX;
	float ey = F_TurretY - F_MatchboxY;
	float ez = F_TurretZ - F_MatchboxZ;

	float eLength = sqrt(ex * ex + ey * ey + ez * ez);

	if (eLength < F_CarCollisionArea + 200)
	{
		return true;
	}

	return false;
}

bool TurretFiring(IModel* F_Matchbox, IModel* F_Turret, IModel* F_Bullet, float F_MatchboxX, float F_MatchboxY, float F_MatchboxZ, float F_CarCollisionArea, float F_BulletCollisionArea, float F_BulletLim, bool F_TargetPos) {

	// Turret looking at matchbox
	F_Turret->LookAt(F_Matchbox);

	if (F_TargetPos == true) {

		F_Bullet->LookAt(F_Matchbox);
		F_TargetPos = false;
	}
	
	// Fire bullet
	F_Bullet->MoveLocalZ(0.1);

	// Collision of Bullet
	float P2x = F_Bullet->GetX();
	float P2y = F_Bullet->GetY();
	float P2z = F_Bullet->GetZ();

	float ex = P2x - F_MatchboxX;
	float ey = P2y - F_MatchboxY;
	float ez = P2z - F_MatchboxZ;

	float eLength = sqrt(ex * ex + ey * ey + ez * ez);

	if (eLength < F_CarCollisionArea + F_BulletCollisionArea)
	{
		// do something
		F_TargetPos = true;

		// reset bullet position
		return true;

	}
	else if (eLength < F_BulletLim)
	{
		// do nothing
		F_TargetPos = true;

		// reset bullet position
		return false;

	}
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed(1080, 720);

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	myEngine->Timer();

	/**** Set up your scene here ****/

#pragma region Creating camera
	// camera 
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 80, -160);
	camera->RotateX(25);
#pragma endregion

#pragma region Loading Meshes
	// meshs
	IMesh* MatchboxMesh = myEngine->LoadMesh("Matchbox.x");
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IMesh* MatchMesh = myEngine->LoadMesh("Match.x");
	IMesh* TwoPenceMesh = myEngine->LoadMesh("TwoPence.x");
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IMesh* TurretMesh = myEngine->LoadMesh("Turret.x");
	IMesh* BulletMesh = myEngine->LoadMesh("Bullet.x");

#pragma endregion

#pragma region Creating Models
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

	// Turret models
	IModel* Turret = TurretMesh->CreateModel(200, 25, 300);
	IModel* Bullet = BulletMesh->CreateModel(0, 0, 0);

#pragma endregion

#pragma region Matchbox car setup
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
#pragma endregion

#pragma region Matchbox car variables
	// MatchBox car variables
	float Speed = 1.0f;
	float RotationSpeed = 0.5f;
	float RotationLimit = 25.0f;
	float MatchFrontRot = 0.0f;
	float SteerAngle = 0.0f;
	float SteerAngleIncrease = 0.01f;

	// Const Car calculation
	const float V = 25.0f;

	// collision
	float CarCollisionArea = 30.0f;
	float MatchboxX = Matchbox->GetX();
	float MatchboxY = Matchbox->GetY();
	float MatchboxZ = Matchbox->GetZ();

	bool MatchBoxPos = true;

#pragma endregion

#pragma region Turret variables

	// collision TurretBullet
	float BulletCollisionArea = 1.0f;
	float TurretX = Turret->GetX();
	float TurretY = Turret->GetY();
	float TurretZ = Turret->GetZ();

	bool FiringZone = false;
	bool BulletFired = false;

	// setting bullet position
	Bullet->SetPosition(Turret->GetX(), Turret->GetY(), Turret->GetZ());
	float BulletLim = 200.0f;
	bool TargetPos = true;

#pragma endregion

	// text test
	IFont* Font_Debug = myEngine->LoadFont("Comic Sans MS", 40);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		float dt = myEngine->Timer();
		float ds = V*dt;

#pragma region Debug Text
		// Debug Text
		stringstream S_Debug;
		S_Debug << "Debug dt: " << dt;
		Font_Debug->Draw(S_Debug.str(), 30, 1025);
#pragma endregion

#pragma region Matchbox movement

		// front movement
		if (myEngine->KeyHeld(Key_Up))
		{
			MatchboxFront(Matchbox, MatchFront, MatchRear, SteerAngle, Speed);
		}
		// back movement
		if (myEngine->KeyHeld(Key_Down))
		{
			MatchboxBack(Matchbox, MatchFront, MatchRear, SteerAngle, Speed);
		}
		// Steer Right
		if (myEngine->KeyHeld(Key_Right) && (MatchFrontRot <= RotationLimit))
		{
			MatchFrontRot = MatchFrontRot + RotationSpeed;
			SteerAngle = MatchboxRight(MatchFrontRot, RotationSpeed, MatchFront, SteerAngle, SteerAngleIncrease);
		}
		// Steer Left
		if (myEngine->KeyHeld(Key_Left) && (MatchFrontRot >= -RotationLimit))
		{
			MatchFrontRot = MatchFrontRot - RotationSpeed;
			SteerAngle = MatchboxLeft(MatchFrontRot, RotationSpeed, MatchFront, SteerAngle, SteerAngleIncrease);
		}

#pragma endregion

		FiringZone = TurretFiringZone(TurretX, TurretY, TurretZ, Matchbox, MatchboxX, MatchboxY, MatchboxZ, CarCollisionArea);

		if (FiringZone == true)
		{
			BulletFired = true;

			if (MatchBoxPos == true)
			{
				MatchboxX = Matchbox->GetX();
				MatchboxY = Matchbox->GetY();
				MatchboxZ = Matchbox->GetZ();

				MatchBoxPos = false;
			}
		}
		
		if (BulletFired == true) {

			if (TurretFiring(Matchbox, Turret, Bullet, MatchboxX, MatchboxY, MatchboxZ, CarCollisionArea, BulletCollisionArea, BulletLim, TargetPos) == true) {

				MatchBoxPos = true;
				myEngine->Stop();
			}
		}

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
