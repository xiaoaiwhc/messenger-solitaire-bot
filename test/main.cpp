#include <iostream>
#include <thread>
#include <chrono>
#include <utility>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <robot.h>

#include "strategy.hpp"
#include "game.hpp"
#include "vision.hpp"
#include "interact.hpp"

int entry_point(int argc, const char *argv[])
{
  static char char_buffer[200];
  robot_h robot = robot_init();

  vision_init(robot);
  interact_init(robot);

  game_state_t game_state = load_initial_game_state();

  std::cout << "Initial state = " << game_state << std::endl;

  try {
    game_state = strategy_init(game_state);
    bool moved;

    do {
      game_state = strategy_step(game_state, &moved);
    } while(moved);
  } catch (std::exception e) {

  }

  std::cout << "Wrapping up " << game_state << std::endl;
  game_state = strategy_term(game_state);

  std::cout << "I AM DONE (not sure if i won the game)" << std::endl;
  std::cout << "Strategy internal state:" << std::endl;
  strategy_print_internal_state();
  std::cout << "Game state: " << std::endl;
  std::cout << game_state << "\n";

  robot_mouse_move(robot, 2000, 100);
  robot_mouse_press(robot, ROBOT_BUTTON1_MASK);
  robot_mouse_release(robot, ROBOT_BUTTON1_MASK);
  robot_free(robot);

  return 0;
}
