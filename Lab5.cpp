// week5.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/

	// Camera
	int GetMouseX();
	int GetMouseY();
	int GetMouseMovementX();
	int GetMouseMovementY();
	ICamera* Camera = myEngine->CreateCamera(kManual, 0, 20, -80);
	float kCameraSpeed = 0.05f;
	float kCameraRotate = 0.02f;
	myEngine->StartMouseCapture();
	bool Run = false;
	Camera->RotateX(10);

	// Meshs and Models
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IModel* Floor = FloorMesh->CreateModel(0, 0, 0);

	IMesh* CubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* Cube = CubeMesh->CreateModel(0, 5, 0);

	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* Arrow = ArrowMesh->CreateModel(0, 15, 0);

	IMesh* SphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* Sphere = SphereMesh->CreateModel(-150, 8, 50);

	// Arrow Rotation Variables
	float ArrowSpeed = 0.1f;
	bool ArrowRotation = false;

	// Cursor Variables
	bool CursorRotation = false;

	// STD transition 
	int a = 0;
	float cubeSpeed = 0.05f;
	int b = 0;
	float cubeBounceSpeed = 0.25f;
	int Bounds = 15;
	double CubeX;
	double CubeZ;
	// 0 == Right 1 == Left
	int CubeS = 0;
	int searchState = 0;

	/*
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IModel* Grid = GridMesh->CreateModel(0, 0.1, 0);
	*/

	// Sphere variables
	float SphereSpeed = 0.5f;
	double SphereX;
	int SphereS = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Quit Game
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// Arrow Rotation Switching
		if (ArrowRotation == false && myEngine->KeyHit(Key_M))
		{
			ArrowRotation = true;
		}
		if (ArrowRotation == true && myEngine->KeyHit(Key_M))
		{
			ArrowRotation = false;
		}

		// Arrow Rotation
		if (ArrowRotation == true)
		{
			int MouseMoveX = myEngine->GetMouseMovementX();
			Arrow->RotateY(MouseMoveX*ArrowSpeed);
			int MouseMoveY = myEngine->GetMouseMovementY();
			Arrow->RotateX(MouseMoveY*ArrowSpeed);
		}

		// Camera Speed Switching
		if (Run == true && myEngine->KeyHit(Key_Shift))
		{
			Run = false;
		}
		if (Run == false && myEngine->KeyHit(Key_Shift))
		{
			Run = true;
		}
		if (Run == true)
		{
			kCameraSpeed = 0.15f;
		}
		else
		{
			kCameraSpeed = 0.05f;
		}

		// Camera Movement
		if (myEngine->KeyHeld(Key_A))
		{
			Camera->MoveLocalX(-kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_D))
		{
			Camera->MoveLocalX(kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_W))
		{
			Camera->MoveLocalZ(kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_S))
		{
			Camera->MoveLocalZ(-kCameraSpeed);
		}

		// Cursor Rotation Switching
		if (CursorRotation == false && myEngine->KeyHit(Key_Q))
		{
			CursorRotation = true;
		}
		if (CursorRotation == true && myEngine->KeyHit(Key_Q))
		{
			CursorRotation = false;
		}

		// Cursor Hide/Show
		if (CursorRotation == false)
		{
			myEngine->StopMouseCapture();
		}
		if (CursorRotation == true)
		{
			myEngine->StartMouseCapture();
		}

		// Reset
		if (myEngine->KeyHit(Key_R))
		{
			Arrow->ResetOrientation();
		}

		// Camera Rotation Switch
		int CameraMoveX = myEngine->GetMouseMovementX();
		Camera->RotateY(CameraMoveX*kCameraRotate);

		//Temp removed mouse Y movement
		/*
		int CameraMoveY = myEngine->GetMouseMovementY();
		Camera->RotateX(CameraMoveY*kCameraRotate);
		*/

		// Finite state Machine for cube

		// case switching
		if (myEngine->KeyHit(Key_0))
		{
			a = 0;
		}
		else if (myEngine->KeyHit(Key_1))
		{
			a = 1;
		}
		else if (myEngine->KeyHit(Key_5))
		{
			a = 2;
		}

		// switch with default state
		switch (a)
		{
			case 0:
			{
				Cube->RotateX(cubeSpeed);
			}
			break;

			case 1:
			{
				Cube->RotateX(-cubeSpeed);
			}
			break;

			default:
			{
				// error state when a!=0 or a!=1 then this happens
				Cube->RotateY(cubeSpeed);
			}
			break;
		}

		CubeX = Cube->GetX();
		CubeZ = Cube->GetZ();

		// example of case switching for cube movement
		if ((b==1)&&(myEngine->KeyHit(Key_B)))
		{
			searchState = 0;
		}
		else if ((b==0)&&(myEngine->KeyHit(Key_B)))
		{
			searchState = 1;
		}

		if (searchState == 0)
		{
			if (CubeX == 0.0 && CubeZ == 0.0)
			{
				b = 0;
			}
		}
		else if(searchState == 1)
		{
			if (CubeX == 0.0 && CubeZ == 0.0)
			{
				b = 1;
			}
		}

		switch (b)
		{
			case 0:
			{
				if (CubeS == 0)
				{
					Cube->MoveX(cubeBounceSpeed);
					if (CubeX == Bounds)
					{
						CubeS = 1;
					}
				}
				else
				{
					Cube->MoveX(-cubeBounceSpeed);
					if (CubeX == -Bounds)
					{
						CubeS = 0;
					}
				}
			}
			break;

			case 1:
			{
				if (CubeS == 0)
				{
					Cube->MoveZ(cubeBounceSpeed);
					if (CubeZ == Bounds)
					{
						CubeS = 1;
					}
				}
				else
				{
					Cube->MoveZ(-cubeBounceSpeed);
					if (CubeZ == -Bounds)
					{
						CubeS = 0;
					}
				}
			}
			break;

			default:
				break;
		}

		SphereX = Sphere->GetX();

		switch (SphereS)
		{
			case 0:
			{
				Sphere->RotateZ(-SphereSpeed);
				Sphere->MoveX(SphereSpeed);

				if (SphereX == 150)
				{
					SphereS = 1;
				}
			}
			break;

			case 1:
			{
				Sphere->RotateZ(SphereSpeed);
				Sphere->MoveX(-SphereSpeed);

				if (SphereX == -150)
				{
					SphereS = 0;
				}
			}
			break;

			default:
				break;
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();

}