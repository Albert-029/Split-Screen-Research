#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "cmath"
#include "Timer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start(int x, int y, int z, float angle, PLAYER p, Color color)
{
	LOG("Loading player");

	player = p;

	initialPosition = { (float)x,(float)y,(float)z };

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 3.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.5f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	car.angle = angle;
	car.color = color;

	// Wheel properties ---------------------------------------
	float connection_height = 2.0f;
	float wheel_radius = 0.3f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 2.0f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(x, y, z);
	vehicle->SetVelocityZero();
	position = vehicle->GetPos();

	arrow.SetPos(x, y + 3, z);

	timer.Start();
	timer2.Start();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	vehicle = nullptr;

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	float km = vehicle->GetKmh();
	if (!App->scene_intro->start) {
		if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && player == PLAYER2))
		{
			if (!App->audio->playingFX(CAR1) && player == PLAYER1) {
				App->audio->StopFx(3);
				App->audio->PlayFx(CAR1, 1, 1);
			}

			if (!App->audio->playingFX(CAR2) && player == PLAYER2) {
				App->audio->StopFx(4);
				App->audio->PlayFx(CAR2, 1, 2);
			}

			if (km < 0.0f)
				acceleration = MAX_ACCELERATION * 10; //brake = BRAKE_POWER;
			else if (km < 120)
				acceleration = MAX_ACCELERATION * 2;
		}

		if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && player == PLAYER2)) {

			if (player == PLAYER1)
				App->audio->StopFx(1);
			if (player == PLAYER2)
				App->audio->StopFx(2);
		}

		if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && player == PLAYER2))
		{
			if (App->audio->playingFX(3) && player == PLAYER1) {
				App->audio->StopFx(3);
				App->audio->PlayFx(CAR_BACK1, 1, 1);
			}

			if (App->audio->playingFX(4) && player == PLAYER2) {
				App->audio->StopFx(4);
				App->audio->PlayFx(CAR_BACK2, 1, 2);
			}

			if (km > 0.0f)
				acceleration = -MAX_ACCELERATION * 10;
			else
				acceleration = -MAX_ACCELERATION * 2;
		}
		if (((App->input->GetKey(SDL_SCANCODE_DOWN) == App->input->GetKey(SDL_SCANCODE_UP)) && player == PLAYER1) ||
			((App->input->GetKey(SDL_SCANCODE_S) == App->input->GetKey(SDL_SCANCODE_W)) && player == PLAYER2)) {

			if (!App->audio->playingFX(3) && player == PLAYER1) {
				App->audio->StopFx(1);
				App->audio->PlayFx(CAR_IDLE1, 1, 3);
			}

			if (!App->audio->playingFX(4) && player == PLAYER2) {
				App->audio->StopFx(2);
				App->audio->PlayFx(CAR_IDLE2, 1, 4);
			}

			if (km > 0.0f)
				acceleration = -MAX_ACCELERATION;
			else if (km < 0.0f)
				acceleration = MAX_ACCELERATION;
		}

		if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player == PLAYER2))
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;

		}

		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player == PLAYER2))
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if ((App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN && player == PLAYER1) ||
			(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && player == PLAYER2))
		{
			if (timer2.Read() > 1500) {
				App->audio->PlayFx(JUMP);
				timer2.Start();
				vehicle->Push(0, 5000, 0);
			}
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	position = vehicle->GetPos();

	arrow.SetPos(vehicle->GetPos().x, vehicle->GetPos().y + 2.5, vehicle->GetPos().z);

	Draw();

	return UPDATE_CONTINUE;
}

bool ModulePlayer::Draw() {
	vehicle->Render();

	return true;
}




