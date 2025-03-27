
#include "doge.h"



Doge::Doge(SDL_Renderer* renderer,const char* filePath, int startX, int startY, bool a) {
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        std::cout << "Lỗi tải ảnh Doge: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }

    filepath = filePath;// Lấy đường dẫn dể kiểm tra powerup
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    velocity = -7.3;

    x = startX;
    y = startY;

    src = { 0, 0, BIRD_WIDTH, BIRD_HEIGHT };// Rect lấy bird từ image
    dest = { startX, startY, BIRD_WIDTH, BIRD_HEIGHT };// Rect vẽ bird   
    if (!a) SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);// Nếu không phải bird vẽ cả hình ảnh
}

Doge::~Doge() {
    SDL_DestroyTexture(texture);
}

int Doge::getX() {
    return dest.x;
}
int Doge::getY() {
    return dest.y;
}

void Doge::set_x(int x) {
    dest.x = x;
}
void Doge::set_y(int y) {
    dest.y = y;
}

SDL_Rect Doge::getRect() {
    return dest;
}

std::string Doge::getFilePath() {
    return filepath;
}

void Doge::jump() {
    time = 0;        // Reset thời gian rơi
    y = dest.y;     // Lưu vị trí bắt đầu nhảy
    angle = -25;     // Nghiêng Doge lên khi nhảy
}

void Doge::increase_velocity() {
    velocity -= 1.8;
}

void Doge::decrease_velocity() {
    velocity += 1.8;
}


void Doge::update() {
    if (time == 0) {
        y = dest.y;
        angle = -25;
    }

    // Cập nhật vị trí theo công thức vật lý
    dest.y = y + time * time * 0.18 + velocity * time;
    time++;

    // Giới hạn góc xoay khi rơi
    if (time > 30) {
        angle += 3;
        if (angle > 90) angle = 90; // không xoay quá 90 độ
    }

    // Kiểm tra nếu chạm đất
    if (dest.y + BIRD_HEIGHT >= BACKGROUND_HEIGHT) {
        dest.y = BACKGROUND_HEIGHT - BIRD_HEIGHT; // Đặt Doge đúng vị trí đất
        time = 0; // Reset thời gian rơi
    }    
}
void Doge::update_src() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastUpdate + frameDelay) {
        currentFrame = (currentFrame + 1) % 6;// 6 Frame 1 ảnh
        lastUpdate = currentTime;
    }

}

void Doge::render_bird(SDL_Renderer* renderer) {
    src = { currentFrame % 3 * BIRD_WIDTH, currentFrame / 3 * BIRD_HEIGHT, BIRD_WIDTH, BIRD_HEIGHT };
    SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, nullptr, SDL_FLIP_NONE);
}

void Doge::render(SDL_Renderer* renderer) {
   
    SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, nullptr, SDL_FLIP_NONE);
}

void Doge::update_powerup(){
    dest.y += 1;
    dest.x -= 2;
}






