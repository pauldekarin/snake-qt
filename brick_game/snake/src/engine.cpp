#include "../include/engine.hpp"

std::shared_ptr<__engine> __engine::__instance(){
    static std::shared_ptr<__engine> __instance = std::shared_ptr<__engine>(new __engine());
    return __instance;
}

void userInput(UserAction_t action, bool hold){
    std::shared_ptr<__engine> engine_ = __engine::__instance();
    engine_->user_input(action, hold);
}
GameInfo_t updateCurrentState(){
    std::shared_ptr<__engine> __instance = __engine::__instance();
    __instance->update();

    return __instance->data();
}