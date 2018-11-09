// Lab6.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#define Left_State 0;
#define Down_State 1;
#define Right_State 2;
#define Up_State 3;

#define Left_State2 0;
#define Down_State2 1;
#define Right_State2 2;
#define Up_State2 3;


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	
	// Camera
	ICamera* Camera = myEngine->CreateCamera(kManual, 0, 120, -40);

	// Meshs
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IMesh* SphereMesh = myEngine->LoadMesh("Sphere.x");

	// Models
	IModel* Floor = FloorMesh->CreateModel(0, 0, 0);
	IModel* Grid = GridMesh->CreateModel(0, 0.1, 0);
	IModel* Sphere = SphereMesh->CreateModel(40, 5, 40);
	IModel* Sphere2 = SphereMesh->CreateModel(-40, 5, -40);

	// Setting up camera state
	Camera->RotateLocalX(75);

	// Configuring Sphere Behavior
	int SphereState = Left_State;
	float SphereSpeed = 0.06;
	float SphereRot = 3 * SphereSpeed;
	float MinSphereSpeed = 0.06;
	float MaxSphereSpeed = 2.00;

	// Configuring Sphere 2 Behavior
	int SphereState2 = Right_State2;
	float SphereSpeed2 = 0.06;
	float SphereRot2 = 3 * SphereSpeed2;
	float MinSphereSpeed2 = 0.06;
	float MaxSphereSpeed2 = 2.00;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Quiting the Game
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// Sphere Speed Adjust Button
		if (myEngine->KeyHit(Key_Up))
		{
			if (SphereSpeed <= MaxSphereSpeed)
			{
				SphereSpeed = SphereSpeed + 0.03;
				SphereRot = SphereRot + 0.09;
			}
		} 
		else if (myEngine->KeyHit(Key_Down))
		{
			if (SphereSpeed >= MinSphereSpeed)
			{
				SphereSpeed = SphereSpeed - 0.03;

				if (SphereRot >= 0.09)
				{
					SphereRot = SphereRot - 0.09;
				}
			}
		}

		// Sphere 2 Speed Adjust Button
		if (myEngine->KeyHit(Key_Right))
		{
			if (SphereSpeed2 <= MaxSphereSpeed2)
			{
				SphereSpeed2 = SphereSpeed2 + 0.03;
				SphereRot2 = SphereRot2 + 0.09;
			}
		}
		else if (myEngine->KeyHit(Key_Left))
		{
			if (SphereSpeed2 >= MinSphereSpeed2)
			{
				SphereSpeed2 = SphereSpeed2 - 0.03;

				if (SphereRot2 >= 0.09)
				{
					SphereRot2 = SphereRot2 - 0.09;
				}
			}
		}

		// Switch Sphere 1 State
		switch (SphereState)
		{
			case 0:
			{
				Sphere->MoveX(-SphereSpeed);
				Sphere->RotateZ(SphereRot);

				if (Sphere->GetX() <= -40)
				{
					SphereState = Down_State;
				}
				break;
			}

			case 1:
			{
				Sphere->MoveZ(-SphereSpeed);
				Sphere->RotateX(-SphereRot);

				if (Sphere->GetZ() <= -40)
				{
					SphereState = Right_State;
				}
				break;
			}

			case 2:
			{
				Sphere->MoveX(SphereSpeed);
				Sphere->RotateZ(-SphereRot);

				if (Sphere->GetX() >= 40)
				{
					SphereState = Up_State;
				}
				break;
			}

			case 3:
			{
				Sphere->MoveZ(SphereSpeed);
				Sphere->RotateX(SphereRot);

				if (Sphere->GetZ() >= 40)
				{
					SphereState = Left_State;
				}
				break;
			}

		}

		// Switch Sphere 2 State
		switch (SphereState2)
		{
		case 0:
		{
			Sphere2->MoveX(-SphereSpeed2);
			Sphere2->RotateZ(SphereRot2);

			if (Sphere2->GetX() <= -40)
			{
				SphereState2 = Down_State2;
			}
			break;
		}

		case 1:
		{
			Sphere2->MoveZ(-SphereSpeed2);
			Sphere2->RotateX(-SphereRot2);

			if (Sphere2->GetZ() <= -40)
			{
				SphereState2 = Right_State2;
			}
			break;
		}

		case 2:
		{
			Sphere2->MoveX(SphereSpeed2);
			Sphere2->RotateZ(-SphereRot2);

			if (Sphere2->GetX() >= 40)
			{
				SphereState2 = Up_State2;
			}
			break;
		}

		case 3:
		{
			Sphere2->MoveZ(SphereSpeed2);
			Sphere2->RotateX(SphereRot2);

			if (Sphere2->GetZ() >= 40)
			{
				SphereState2 = Left_State2;
			}
			break;
		}

		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
