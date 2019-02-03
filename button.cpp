#include "button.hpp"
#include "textures.hpp"
#include "text.hpp"

Button::Button(Dimensions dimensions,
               Position positon,
               std::string caption,
               int buttonId,
               bool isVisible)
    : font(TTF_OpenFont("Fonts//font.ttf", 40)),
      textColor({255, 255, 255, 0}),
      lineTexture(getTexture("Images//buttonline.png")),
      isUnderMouseMotion(false),
      isUnderClick(false),
      needsDrawing(true)
{
    this->dimensions = dimensions;
    this->position = positon;
    this->caption = caption;
    this->buttonId = buttonId;
    this->isVisible = isVisible;
}

void Button::draw()
{
    if (needsDrawing and isVisible)
    {
        if (isUnderMouseMotion) glColor3f(0.0, 0.2, 0.4);
        else glColor3f(0.0, 0.2, 0.6);

        drawRectangle({dimensions.width, dimensions.height},
                      {position.x, position.y});
        drawBorder();
        renderCaption();
        needsDrawing = false;
    }
}

void Button::forceDraw()
{
    needsDrawing = true;
}

std::string Button::getCaption()
{
    return caption;
}

void Button::immediatelyDraw()
{
    forceDraw();
    draw();
    SDL_GL_SwapBuffers();;
}

void Button::setCaption(std::string caption)
{
    this->caption = caption;
}

void Button::renderCaption()
{
    SDL_GL_RenderText(caption.c_str(),
                      font,
                      textColor,
                      position.x + dimensions.width/2.0,
                      position.y - dimensions.height/4.0,
                      dimensions.height);
}

void Button::drawBorder()
{
    turnOnTextureMode(lineTexture);
    drawVerticalBorders();
    drawHorizontalBorders();
    turnOffTextureMode();
}

void Button::drawVerticalBorders()
{
    const int fragments = 20;
    const double borderWidth = dimensions.width / 30;
    const double fragmentHeight = dimensions.height / fragments;

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {borderWidth, fragmentHeight},
            {position.x, position.y - i * fragmentHeight});
    }

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {borderWidth, fragmentHeight},
            {position.x + dimensions.width - borderWidth,
             position.y - i * fragmentHeight});
    }
}

void Button::drawHorizontalBorders()
{
    const int fragments = 40;
    const double borderHeight = dimensions.height / 7;
    const double fragmentWidth = dimensions.width / fragments;

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {fragmentWidth, borderHeight},
            {position.x + i * fragmentWidth, position.y});
    }

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {fragmentWidth, borderHeight},
            {position.x + i * fragmentWidth,
             position.y - dimensions.height + borderHeight});
    }
}

void Button::updateMotion(const Position &position)
{
    bool wasUnderMotion = isUnderMouseMotion;
    isUnderMouseMotion = isInside(position);

    needsDrawing = wasUnderMotion != isUnderMouseMotion or needsDrawing;
}

void Button::updateUnderClick()
{
    isUnderClick = isUnderMouseMotion;
}

bool Button::isClicked()
{
    return isUnderClick;
}

bool Button::isInside(const Position &position)
{
    return position.x >= this->position.x and
           position.x <= this->position.x + this->dimensions.width and
           position.y <= this->position.y and
           position.y >= this->position.y - this->dimensions.height;
}

int Button::getButtonId()
{
    return buttonId;
}
