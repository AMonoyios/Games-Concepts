// Monoyios.cpp: A program using the TL-Engine

// Milestones Achieved:
// Milestone 1: Basic Scene Setup -DONE-
// Milestone 2: Camera Motion -DONE-
// Milestone 3: Basic FSMs and Camera Motion -DONE-
// Milestone 4: More User Controls -DONE-
// Milestone 5: Editing the Spheres' Behaviour --

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <functional>
#include <stdio.h>
using namespace tle;

#define Sphere1_Move_Right 0;
#define Sphere1_Move_Down 1;
#define Sphere1_Move_Left 2;
#define Sphere1_Move_Up 3;

#define Sphere2_Move_Right 0;
#define Sphere2_Move_Down 1;
#define Sphere2_Move_Left 2;
#define Sphere2_Move_Up 3;

#define Pause_Off 0;
#define Pause_On 1;

#define ClockwiseMovement 0;
#define AntiClockwiseMovement 1;

#define Pointer_Off 0;
#define Pointer_On 1;

double test(double test1, double test2, double test3)
{

}

void main()
{

	

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("./media");

	/**** Set up your scene here ****/

	// Setting up camera
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 120, -200);

	// Camera Variables
	camera->RotateX(20);
	float CameraSpeed = 0.2f;
	float MouseSensitivity = 0.5f;
	float MouseLocX = myEngine->GetMouseMovementX();
	float MouseLocY = myEngine->GetMouseMovementY();

	// Loading Meshs
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IMesh* SphereMesh = myEngine->LoadMesh("sphere.x");

	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");

	// Creating Models
	IModel* Floor = FloorMesh->CreateModel(0, 0, 0);
	IModel* Sphere1 = SphereMesh->CreateModel(-50, 10, 0);
	IModel* Sphere2 = SphereMesh->CreateModel(50, 10, 50);

	IModel* Grid = GridMesh->CreateModel(0, 0.1, 0);

	// Changing Sphere2 skin
	Sphere2->SetSkin("indigo.png");

	// Sphere Variables
	const float kGameSpeed = 1.0;
	const int Sphere_X1_Limit = 50;
	const int Sphere_X2_Limit = -50;
	const int Sphere_Z1_Limit = 0;
	const int Sphere_Z2_Limit = 100;
	const float sphereSpeedMax = 2.5;
	const float sphereSpeedMin = 0.01;
	const int sphereElevationMax = 20;
	const int sphereElevationMin = 10;
	double sphereSpeed = 0.05;
	double sphereSpeedStepping = 0.02;
	double sphereRollStepping = 0.01f;
	double sphereElevationSpeed = 0.02;
	double sphereRotatingSpeed = sphereSpeed * 3;

	// Sphere States
	int Sphere1State = Sphere1_Move_Up;
	int Sphere2State = Sphere2_Move_Down;
	int SphereTouchingGround = 10;
	int SphereDirectionState = ClockwiseMovement;

	// Mouse pointer State
	int PointerState = Pointer_Off;

	// Pause State
	int PauseState = Pause_Off;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Camera Motion
		// Camera Forward motion
		if (myEngine->KeyHeld(Key_W))
		{
			camera->MoveLocalZ(CameraSpeed);
		}
		// Camera Backwards motion
		if (myEngine->KeyHeld(Key_S))
		{
			camera->MoveLocalZ(-CameraSpeed);
		}
		// Camera Leftwards motion
		if (myEngine->KeyHeld(Key_A))
		{
			camera->MoveLocalX(-CameraSpeed);
		}
		// Camera Rightwards motion
		if (myEngine->KeyHeld(Key_D))
		{
			camera->MoveLocalX(CameraSpeed);
		}
		// Camera Left-Right Yaw
		MouseLocX = myEngine->GetMouseMovementX();
		if (MouseLocX > myEngine->GetMouseMovementX())
		{
			camera->RotateLocalY(MouseSensitivity);
		}
		else if (MouseLocX < myEngine->GetMouseMovementX())
		{
			camera->RotateLocalY(-MouseSensitivity);
		}
		// Camera Up-Down Pitch
		MouseLocY = myEngine->GetMouseMovementY();
		if (MouseLocY > myEngine->GetMouseMovementY())
		{
			camera->RotateLocalX(MouseSensitivity);
		}
		else if (MouseLocY < myEngine->GetMouseMovementY())
		{
			camera->RotateLocalX(-MouseSensitivity);
		}

		// Additional camera/mouse functions
		// Camera Roll motion
		if (myEngine->KeyHeld(Key_Period))
		{
			camera->RotateLocalZ(-sphereRollStepping);
		}
		else if (myEngine->KeyHeld(Key_Comma))
		{
			camera->RotateLocalZ(sphereRollStepping);
		}
		// Mouse Pointer toggle
		switch (PointerState)
		{
		case 0:
		{
			if (myEngine->KeyHit(Key_M))
			{
				myEngine->StartMouseCapture();

				PointerState = Pointer_On;
			}
			break;
		}
		case 1:
		{
			if (myEngine->KeyHit(Key_M))
			{
				myEngine->StopMouseCapture();

				PointerState = Pointer_Off;
			}
			break;
		}
		default:
			break;
		}

		// Pause Game
		switch (PauseState)
		{
		case 0:
		{
			// Sphere Movement
			switch (SphereDirectionState)
			{
				// Clockwise movement of spheres
				case 0:
				{
					// Checking if Anticlockwise movement was toggled
					if (myEngine->KeyHit(Key_R))
					{
						SphereDirectionState = AntiClockwiseMovement;
						break;
					}

					// Sphere 1 Movement
					switch (Sphere1State)
					{
					case 0:
					{
						// Move Sphere 1 right-wards
						Sphere1->MoveX(kGameSpeed * sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateZ(-sphereRotatingSpeed);
						}

						if (Sphere1->GetX() >= Sphere_X1_Limit)
						{
							Sphere1State = Sphere1_Move_Down;
						}

						break;
					}
					case 1:
					{
						// Move Sphere 1 down-wards
						Sphere1->MoveZ(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateX(-sphereRotatingSpeed);
						}

						if (Sphere1->GetZ() <= Sphere_Z1_Limit)
						{
							Sphere1State = Sphere1_Move_Left;
						}

						break;
					}
					case 2:
					{
						// Move Sphere 1 left-wards
						Sphere1->MoveX(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateZ(sphereRotatingSpeed);
						}

						if (Sphere1->GetX() <= Sphere_X2_Limit)
						{
							Sphere1State = Sphere1_Move_Up;
						}

						break;
					}
					case 3:
					{
						// Move Sphere 1 up-wards
						Sphere1->MoveZ(kGameSpeed * sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateX(sphereRotatingSpeed);
						}

						if (Sphere1->GetZ() >= Sphere_Z2_Limit)
						{
							Sphere1State = Sphere1_Move_Right;
						}

						break;
					}
					}

					// Sphere 2 Movement
					switch (Sphere2State)
					{
					case 0:
					{
						// Move Sphere 2 right-wards
						Sphere2->MoveX(kGameSpeed * sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateZ(-sphereRotatingSpeed);
						}

						if (Sphere2->GetX() >= Sphere_X1_Limit)
						{
							Sphere2State = Sphere2_Move_Down;
						}

						break;
					}
					case 1:
					{
						// Move Sphere 2 down-wards
						Sphere2->MoveZ(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateX(-sphereRotatingSpeed);
						}

						if (Sphere2->GetZ() <= Sphere_Z1_Limit)
						{
							Sphere2State = Sphere2_Move_Left;
						}

						break;
					}
					case 2:
					{
						// Move Sphere 2 left-wards
						Sphere2->MoveX(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateZ(sphereRotatingSpeed);
						}

						if (Sphere2->GetX() <= Sphere_X2_Limit)
						{
							Sphere2State = Sphere2_Move_Up;
						}

						break;
					}
					case 3:
					{
						// Move Sphere 2 up-wards
						Sphere2->MoveZ(kGameSpeed * sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateX(sphereRotatingSpeed);
						}

						if (Sphere2->GetZ() >= Sphere_Z2_Limit)
						{
							Sphere2State = Sphere2_Move_Right;
						}

						break;
					}
					}
					
					break;
				}

				// Anticlockwise movement of spheres 
				case 1:
				{
					// Checking if Clockwise movement was toggled
					if (myEngine->KeyHit(Key_R))
					{
						SphereDirectionState = ClockwiseMovement;
						break;
					}

					// Sphere 1 Movement
					switch (Sphere1State)
					{
					case 0:
					{
						// Move Sphere 1 left-wards
						Sphere1->MoveX(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateZ(-sphereRotatingSpeed);
						}

						if (Sphere1->GetX() <= Sphere_X2_Limit)
						{
							Sphere1State = Sphere1_Move_Down;
						}

						break;
					}
					case 1:
					{
						// Move Sphere 1 down-wards
						Sphere1->MoveZ(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateX(-sphereRotatingSpeed);
						}

						if (Sphere1->GetZ() <= Sphere_Z1_Limit)
						{
							Sphere1State = Sphere1_Move_Left;
						}

						break;
					}
					case 2:
					{
						// Move Sphere 1 right-wards
						Sphere1->MoveX(kGameSpeed * sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateZ(sphereRotatingSpeed);
						}

						if (Sphere1->GetX() >= Sphere_X1_Limit)
						{
							Sphere1State = Sphere1_Move_Up;
						}

						break;
					}
					case 3:
					{
						// Move Sphere 1 up-wards
						Sphere1->MoveZ(kGameSpeed * sphereSpeed);

						// Checking if Sphere 1 is touching the ground
						if (Sphere1->GetY() <= SphereTouchingGround)
						{
							Sphere1->RotateX(sphereRotatingSpeed);
						}

						if (Sphere1->GetZ() >= Sphere_Z2_Limit)
						{
							Sphere1State = Sphere1_Move_Right;
						}

						break;
					}
					}

					// Sphere 2 Movement
					switch (Sphere2State)
					{
					case 0:
					{
						// Move Sphere 2 left-wards
						Sphere2->MoveX(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateZ(-sphereRotatingSpeed);
						}

						if (Sphere2->GetX() <= Sphere_X2_Limit)
						{
							Sphere2State = Sphere2_Move_Down;
						}

						break;
					}
					case 1:
					{
						// Move Sphere 2 down-wards
						Sphere2->MoveZ(kGameSpeed * -sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateX(-sphereRotatingSpeed);
						}

						if (Sphere2->GetZ() <= Sphere_Z1_Limit)
						{
							Sphere2State = Sphere2_Move_Left;
						}

						break;
					}
					case 2:
					{
						// Move Sphere 2 right-wards
						Sphere2->MoveX(kGameSpeed * sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateZ(sphereRotatingSpeed);
						}

						if (Sphere2->GetX() >= Sphere_X1_Limit)
						{
							Sphere2State = Sphere2_Move_Up;
						}

						break;
					}
					case 3:
					{
						// Move Sphere 2 up-wards
						Sphere2->MoveZ(kGameSpeed * sphereSpeed);

						// Checking if Sphere 2 is touching the ground
						if (Sphere2->GetY() <= SphereTouchingGround)
						{
							Sphere2->RotateX(sphereRotatingSpeed);
						}

						if (Sphere2->GetZ() >= Sphere_Z2_Limit)
						{
							Sphere2State = Sphere2_Move_Right;
						}

						break;
					}
					}
					
					break;
				}

			default:
				break;
			}
			
			// Sphere Speed Adjust
			// Sphere increase speed
			if (myEngine->KeyHit(Key_Z) && (sphereSpeed + sphereSpeedStepping)<sphereSpeedMax)
			{
				sphereSpeed += sphereSpeedStepping;
			}
			// Sphere decrease speed
			else if (myEngine->KeyHit(Key_X) && (sphereSpeed - sphereSpeedStepping)>sphereSpeedMin)
			{
				sphereSpeed -= sphereSpeedStepping;
			}
			// Getting the rotation of the spheres 
			sphereRotatingSpeed = sphereSpeed * 3;

			// Spheres elevation control
			// Sphere elevating up
			if (myEngine->KeyHeld(Key_Up) && (Sphere1->GetY() + sphereElevationSpeed)<sphereElevationMax)
			{
				Sphere1->MoveY(sphereElevationSpeed);
				Sphere2->MoveY(sphereElevationSpeed);
			}
			// Sphere elevating down
			else if (myEngine->KeyHeld(Key_Down) && (Sphere1->GetY() - sphereElevationSpeed)>sphereElevationMin)
			{
				Sphere1->MoveY(-sphereElevationSpeed);
				Sphere2->MoveY(-sphereElevationSpeed);
			}

			// Checking if player paused game
			if (myEngine->KeyHit(Key_P))
			{
				PauseState = Pause_On
			}

			break;
		}
		case 1:
		{
			// Checking if player unpaused game
			if (myEngine->KeyHit(Key_P))
			{
				PauseState = Pause_Off;
			}

			break;
		}
		default:
			break;
		}

		// Stoping Game
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}