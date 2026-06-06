enum game_manager_state {
    START_GAME_SCREEN, //on first boot
    GAME_IN_PLAY,
    GAME_OVER_SCREEN, //asks you to input your name
};

extern enum game_manager_state game_manager_state;