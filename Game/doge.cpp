
#include "doge.h"



Doge::Doge(SDL_Renderer* renderer,const char* filePath, int startX, int startY) {
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        std::cout << "Lỗi tải ảnh Doge: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }

    filepath = filePath;
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    x = startX;
    y = startY;

    dest = { startX, startY, DOGE_WIDTH, DOGE_HEIGHT };
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);    
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
SDL_Rect Doge::getRect() {
    return dest;
}

std::string Doge::getFilePath() {
    return filepath;
}

void Doge::jump() {
    velocity = -7.3; // Nhảy lên với lực ban đầu 
    time = 0;        // Reset thời gian rơi
    y = dest.y;     // Lưu vị trí bắt đầu nhảy
    angle = -25;     // Nghiêng Doge lên khi nhảy
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
    if (dest.y + DOGE_HEIGHT >= BACKGROUND_HEIGHT) {
        dest.y = BACKGROUND_HEIGHT - DOGE_HEIGHT; // Đặt Doge đúng vị trí đất
        time = 0; // Reset thời gian rơi
    }
}


void Doge::render(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, nullptr, SDL_FLIP_NONE);
}

void Doge::updateChampion() {
    if (growing) {
        championScale += 0.01; // Tăng kích thước mỗi frame
        if (championScale >= 1) { // Giới hạn kích thước tối đa
            championScale = 1;
            growing = false;
        }

        // Áp dụng hiệu ứng phóng to
        dest.w = static_cast<int>(300 * championScale);
        dest.h = static_cast<int>(300 * championScale);
        dest.x = x - (dest.w - 300) / 2; // Giữ vị trí trung tâm
        dest.y = y + (dest.h - 300) / 2;
    }
}

void Doge::update_powerup(){
    dest.y += 1;
    dest.x -= 2;
}




