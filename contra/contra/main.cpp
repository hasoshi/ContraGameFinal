
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "TextObject.h"
#include "ImpTimer.h"
//#include "ThreatsObject.h"

BaseObject g_background;
TTF_Font* font_time = NULL;

bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("Game contra - HHL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL) {
		success = false;
	}
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}

		if (TTF_Init() == -1)
		{
			success = false;
		}
		font_time = TTF_OpenFont("font//08634_ClarendonBT.ttf", 20);
		if (font_time == NULL)
		{
			success = false;
		}
	}
	return success;
}

bool LoadBackground() {
	bool ret = g_background.LoadImg("img/background.png", g_screen);
	if (ret == false)
		return false;
	return true;
}

void close() {
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}
//std::vector<ThreatsObject*> MakeThreadList() {

	//std::vector<ThreatsObject*> list_threats;

	//ThreatsObject* threats_objs = new ThreatsObject[20];

	//for (int i = 0; i < 20; i++) {

		//ThreatsObject* p_threat = (threats_objs + i);

		//if (p_threat != NULL) {

			//p_threat->LoadImg("img//threat_level.png", g_screen);
			//p_threat->set_clips();
			//p_threat->set_x_pos(700 + i*1200);
			//p_threat->set_y_pos(250);

			//list_threats.push_back(p_threat);
		//}
	//}
	//return list_threats;

//}
#undef main

int main(int argc, char* argv[]) {

	//imptimer

	ImpTimer fps_timer;


	if (InitData() == false)
		return -1;
	if (LoadBackground() == false)
		return -1;

	GameMap game_map;
	game_map.LoadMap((char*)"map/map01.dat");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();

	//std::vector<ThreatsObject*> threats_list = MakeThreadList();

	//Time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;

	TextObject money_count;
	money_count.SetColor(TextObject::WHITE_TEXT);

	bool is_quit = false;
	while (!is_quit) {
		fps_timer.start(); //fps
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true;
			}

			p_player.HandelInputAction(g_event, g_screen);

		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);
		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);


		/*for (int i = 0; i < threats_list.size(); i++) {
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL) {
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->DoPlayer(map_data);
				p_threat->Show(g_screen);
			}
		}*/
		//Show game time
		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 300 - time_val;
		if (val_time <= 0)
		{
			if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				is_quit = true;
				break;
			}
		}
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
		}
		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; //ms

		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if(delay_time>=0)
			    SDL_Delay(delay_time);
		}
	}


	close();
	return 0;
}