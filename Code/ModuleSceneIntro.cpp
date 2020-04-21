#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"
#include "Color.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "cmath"
#include "ModuleRenderer3D.h"
#include "glut/glut.h"
#include <string>


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("Music/song.ogg");
	App->audio->ChangeMusicVolume(0.6f);
	App->audio->LoadFx("FX/car2.wav");
	App->audio->LoadFx("FX/car3.wav");
	App->audio->LoadFx("FX/car_idle1.wav");
	App->audio->LoadFx("FX/car_idle2.wav");
	App->audio->LoadFx("FX/car_back1.wav");
	App->audio->LoadFx("FX/car_back2.wav");
	App->audio->LoadFx("FX/powerup.wav");
	App->audio->LoadFx("FX/jump.wav");
	App->audio->LoadFx("FX/boost.wav");
	App->audio->LoadFx("FX/tick.wav");
	App->audio->LoadFx("FX/go.wav");
	App->audio->LoadFx("FX/whistle.wav");
	App->audio->LoadFx("FX/goal.wav");

	player1 = new ModulePlayer(App, true);
	player2 = new ModulePlayer(App, true);

	player1->Start(0, 0, 95, 0, PLAYER1, Red);

	player2->Start(10, 0, 95, 0, PLAYER2, Blue);

	player1->vehicle->SetVelocityZero();
	player2->vehicle->SetVelocityZero();

	createMap();
	TimeLeft();

	game.Start();
	endGame = false;
	start = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (p2List_item<Primitive*>* item = map.getFirst(); item; item = item->next)
		delete item->data;
	map.clear();

	for (p2List_item<Cylinder*>* item = cylinders_list1.getFirst(); item; item = item->next)
		delete item->data;
	cylinders_list1.clear();

	for (p2List_item<Cylinder*>* item = cylinders_list2.getFirst(); item; item = item->next)
		delete item->data;
	cylinders_list2.clear();

	for (p2List_item<Cylinder*>* item = time_list.getFirst(); item; item = item->next)
		delete item->data;
	time_list.clear();

	player1->CleanUp();
	player2->CleanUp();

	delete player1;
	delete player2;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (!endGame) {
		time_remaining = game.Read();
		LOG("%i", time_remaining);
		if (time_remaining >= interval && time_remaining != 0)
		{
			p2List_item<Cylinder*>* item = time_list.getFirst();
			for (int i = 0; i < time_list.count() && item->next != nullptr; item = item->next) {
				if (item->data->color.IsBlack())
				{
					item->data->color = Red;
					break;
				}
				else if (item->next->next == nullptr)
				{
					item->next->data->color = Red;
					endGame = true;
					interval = counter;
					App->audio->PlayFx(WHISTLE);
					time_left.Start();
				}

			}
			interval += counter;
		}
	}

	player1->Update(dt);
	player2->Update(dt);

	App->camera->Position.x = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * player1->vehicle->vehicle->getForwardVector().getX();
	App->camera->Position.y = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * player1->vehicle->vehicle->getUpAxis();
	App->camera->Position.z = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * player1->vehicle->vehicle->getForwardVector().getZ();

	float player1_x = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * player1->vehicle->vehicle->getForwardVector().getX();
	float player1_z = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * player1->vehicle->vehicle->getForwardVector().getZ();

	App->camera->LookAt(vec3(player1_x, 1, player1_z));

	//TODO 4 (Assign Camera to Player)
	
	App->camera2->Position.x = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * player2->vehicle->vehicle->getForwardVector().getX();
	App->camera2->Position.y = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * player2->vehicle->vehicle->getUpAxis();
	App->camera2->Position.z = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * player2->vehicle->vehicle->getForwardVector().getZ();

	float player2_x = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * player2->vehicle->vehicle->getForwardVector().getX();
	float player2_z = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * player2->vehicle->vehicle->getForwardVector().getZ();

	App->camera2->LookAt(vec3(player2_x, 1, player2_z));
	
	if (endGame && !start)
		resetLevel();
	else if(!endGame && start)
		startRound();
	
	char title[80];
	sprintf_s(title, "Split Screen");
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::Draw() {
	bool ret = true;
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ret = player1->Draw();
	ret = player2->Draw();

	p2List_item<Primitive*>* item = map.getFirst();
	for (int i = 0; i < map.count() && item != nullptr; item = item->next) {
		item->data->Render();
	}

	p2List_item<Cylinder*>* item2 = cylinders_list1.getFirst();
	for (int i = 0; i < cylinders_list1.count() && item2 != nullptr; item2 = item2->next) {
		item2->data->Render();
	}

	p2List_item<Cylinder*>* item3 = cylinders_list2.getFirst();
	for (int i = 0; i < cylinders_list2.count() && item3 != nullptr; item3 = item3->next) {
		item3->data->Render();
	}

	p2List_item<Cylinder*>* item4 = time_list.getFirst();
	for (int i = 0; i < time_list.count() && item4 != nullptr; item4 = item4->next) {
		item4->data->Render();
	}

	return ret;
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

void ModuleSceneIntro::resetLevel() {
	if (time_left.Read() < 8000) {
		for (p2List_item<Primitive*>* item = map.getFirst(); item != nullptr; item = item->next) {
			if(score1 > score2)
				item->data->color = Orange;
			else if(score2 > score1)
				item->data->color = Blue;
			else if(score1 == score2)
				item->data->color = White;
		}
	}
	else {
		
		for (p2List_item<Cylinder*>* item = time_list.getFirst(); item != nullptr; item = item->next) {
			item->data->color = Black;
		}

		
		for (p2List_item<Cylinder*>* item2 = cylinders_list1.getFirst(); item2 != nullptr; item2 = item2->next) {
			item2->data->color = White;
		}

		
		for (p2List_item<Cylinder*>* item3 = cylinders_list2.getFirst(); item3 != nullptr; item3 = item3->next) {
			item3->data->color = White;
		}

		p2List_item<Primitive*>* item4 = map.getFirst();
		for (int i = 0; i < map.count() && item4 != nullptr; item4 = item4->next, ++i) {
			if(i < map.count()/2)
				item4->data->color = Orange;
			else
				item4->data->color = Blue;
		}
		score1 = 0;
		score2 = 0;
		interval = counter;
		RestartPositions();
		time_left.Start();
		game.Start();
		endGame = false;
		start = true;
	}
}

void ModuleSceneIntro::startRound() {
	uint res = time_left.Read();
	LOG("%i", res);
	if (res >= 1000) {
		second++;
		time_left.Start();
	}

	if ((second == 1 || second == 2) && res >= 1000) {
		App->audio->PlayFx(TICK);
	}
	else if (second == 3 && res >= 1000) {
		App->audio->PlayFx(GO);
		endGame = false;
		start = false;
		second = 0;
	}
}

void ModuleSceneIntro::createMap()
{
	
}



void ModuleSceneIntro::RotateBody(PhysVehicle3D * vehicle)
{
	float x = vehicle->GetPos().x;
	float z = vehicle->GetPos().z;
	float h = sqrt(x*x + z * z);

	float b = x / h;
	float angle = acos(b);
	if (vehicle == player1->vehicle)
		App->physics->rotateVehicle(angle + M_PI/2, vehicle);
	else if (vehicle == player2->vehicle)
		App->physics->rotateVehicle(angle - M_PI / 2, vehicle);
}

void ModuleSceneIntro::Score()
{
	float x = -12.75;
	int y = 11;
	int z = 98.5;

	for (int i = 0; i < 5; ++i)
	{
		Cylinder* score = new Cylinder(2, 0.75);
		score->SetPos(x, y, z);
		score->SetRotation(90, { 0,1,0 });
		score->color = White;
		cylinders_list1.add(score);
		x += 6.5;
	}

	for (int i = 0; i < 5; ++i)
	{
		x -= 6.5;
		Cylinder* score = new Cylinder(2, 0.75);
		score->SetPos(-x, y, -z);
		score->SetRotation(90, { 0,1,0 });
		score->color = White;
		cylinders_list2.add(score);	
	}
}

void ModuleSceneIntro::TimeLeft()
{
	Cylinder* time1 = new Cylinder(1, 0.75);
	time1->SetPos(4, -0.5, 0);
	time1->SetRotation(90, { 0,0,1 });
	time1->color = Black;
	time_list.add(time1);

	Cylinder* time2 = new Cylinder(1, 0.75);
	time2->SetPos(-4, -0.5, 0);
	time2->SetRotation(90, { 0,0,1 });
	time2->color = Black;
	time_list.add(time2);

	Cylinder* time3 = new Cylinder(1, 0.75);
	time3->SetPos(0, -0.5, 4);
	time3->SetRotation(90, { 0,0,1 });
	time3->color = Black;
	time_list.add(time3);

	Cylinder* time4 = new Cylinder(1, 0.75);
	time4->SetPos(0, -0.5, -4);
	time4->SetRotation(90, { 0,0,1 });
	time4->color = Black;
	time_list.add(time4);

	Cylinder* time = new Cylinder(1, 0.75);
	time->SetPos(0, -0.5, 0);
	time->SetRotation(90, { 0,0,1 });
	time->color = Black;
	time_list.add(time);
}

void ModuleSceneIntro::RestartPositions()
{
	player1->vehicle->SetPos(player1->initialPosition.x, player1->initialPosition.y, player1->initialPosition.z);
	player2->vehicle->SetPos(player2->initialPosition.x, player2->initialPosition.y, player2->initialPosition.z);

	RotateBody(player1->vehicle);
	RotateBody(player2->vehicle);

	player1->vehicle->SetVelocityZero();
	player2->vehicle->SetVelocityZero();
}
