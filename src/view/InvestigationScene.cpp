
/*
 
 Copyright 2012 Fabio Rodella
 
 This file is part of LPC2012Game.
 
 LPC2012Game is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 LPC2012Game is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with LPC2012Game.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#include <allegro5/allegro_primitives.h>

#include "InvestigationScene.h"

#include "Room.h"

std::string InvestigationScene::timeToString(long time, bool includeSeconds) {
            
    int hr = time / (60 * 60);
	int min = (((int)time / 60) % 60);
	int sec = (int)time % 60; 
    
    char buff[10];
    
    if (includeSeconds) {
        sprintf(buff, "%.2d:%.2d:%.2d", hr, min, sec);
    } else {
        sprintf(buff, "%.2d:%.2d", hr, min);
    }
    
    std::string ret = buff;
    return ret;
}

InvestigationScene::~InvestigationScene() {
    delete mystery;
    al_destroy_font(font);
    al_destroy_font(fontBig);
}

void InvestigationScene::setupScene() {
    
    std::vector<Point> positions;
    positions.push_back(pointMake(32, 48));
    positions.push_back(pointMake(34, 48));
    positions.push_back(pointMake(36, 48));
    positions.push_back(pointMake(38, 48));
    positions.push_back(pointMake(40, 49));
    positions.push_back(pointMake(40, 51));
    positions.push_back(pointMake(40, 53));
    positions.push_back(pointMake(30, 49));
    positions.push_back(pointMake(30, 51));
    positions.push_back(pointMake(32, 52));
    
    font = al_load_font("res/DejaVuSans.ttf", 16, 0);
    fontBig = al_load_font("res/DejaVuSans.ttf", 26, 0);
    
    std::vector<TilemapLayer *> layers = TilemapLayer::parseTMXFile("res/mansion.tmx");
    std::vector<TilemapLayer *>::iterator it;
    
    TilemapLayer *firstLayer = layers[0];
    
    camera = new Camera(800, 600, firstLayer->getBoundsSize().width, firstLayer->getBoundsSize().height);
    
    for (it = layers.begin(); it < layers.end(); ++it) {
        
        TilemapLayer *layer = (TilemapLayer *) *it;
        layer->setCamera(camera);
        addToDisplayList(layer);
        
        if (layer->isCollision()) {
            collision = layer;
        }
    }
            
    playerSprite = new Spritesheet("res/professor_walk_cycle_no_hat.png", 64, 64);
    playerSprite->setTag(PLAYER_SPRITE_TAG);
    playerSprite->setCamera(camera);
    playerSprite->setPosition(pointMake(35 * 32, 50 * 32));
    playerSprite->setAnchorPoint(pointMake(0.5, 0.9));
    playerSprite->setAutoZOrder(true);
    addToDisplayList(playerSprite);
    
    mystery = new Mystery("res/mansion.xml", (unsigned int) time(0), collision->getData(), collision->getSize().width, collision->getSize().height);
    
    long mysteryTime = 0;
    while (!mystery->ended) {
        mystery->step();
        mysteryTime++;
    }
            
    printf("Total duration: %ld %s\n", mysteryTime, timeToString(mysteryTime, true).c_str());
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator itChars;
    
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        if (!character->dead) {
            int idx = rand() % positions.size();
            Point pos = positions[idx];
            
            character->position = pos;
            
            positions.erase(positions.begin() + idx);
        }
        
        Spritesheet *sprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
        sprite->setTag(character->tag);
        sprite->setCamera(camera);
        sprite->setAnchorPoint(pointMake(0.5, 0.9));
        sprite->setAutoZOrder(true);
        sprite->setFrame(18);
        
        Rect tileRect = collision->getTileRect(character->position.x, character->position.y);
        sprite->setPosition(rectMidPoint(tileRect));
        
        addToDisplayList(sprite);        
    }
    
    actionButton = new Button("action", font, "res/btn_action.png", "res/btn_action_pressed.png");
    actionButton->setZOrder(500);
    actionButton->setAnchorPoint(pointMake(0.5, 1));
    actionButton->setPosition(pointMake(400, 400));
    actionButton->setCamera(camera);
    actionButton->setHandler(this);
    
    addToDisplayList(actionButton);
    
    Spritesheet *bkgRoomLabel = new Spritesheet("res/bkg_room_name.png");
    bkgRoomLabel->setAnchorPoint(pointMake(0.5, 0.5));
    bkgRoomLabel->setPosition(pointMake(400, 40));
    bkgRoomLabel->setZOrder(501);
    
    addToDisplayList(bkgRoomLabel);
    
    currentRoomLabel = new Label("room", font, al_map_rgb(0, 0, 0));
    currentRoomLabel->setAnchorPoint(pointMake(0.5, 0.5));
    currentRoomLabel->setPosition(bkgRoomLabel->getPosition());
    currentRoomLabel->setZOrder(502);
    
    addToDisplayList(currentRoomLabel);
    
    bkgQuestion = new Spritesheet("res/bkg_question.png");
    bkgQuestion->setAnchorPoint(pointMake(0.5, 0.5));
    bkgQuestion->setPosition(pointMake(400, 300));
    bkgQuestion->setZOrder(503);
    bkgQuestion->setVisible(false);
        
    addToDisplayList(bkgQuestion);
    
    questionLabel = new Label("question", font, al_map_rgb(0, 0, 0), 350);
    questionLabel->setAnchorPoint(pointMake(0.5, 0.5));
    questionLabel->setPosition(pointMake(400, 120));
    questionLabel->setZOrder(504);
    questionLabel->setVisible(false);
    
    addToDisplayList(questionLabel);
    
    whenLabel = new Label("00:00 and 00:15", fontBig, al_map_rgb(0, 0, 0));
    whenLabel->setAnchorPoint(pointMake(0.5, 0.5));
    whenLabel->setPosition(pointMake(400, 300));
    whenLabel->setZOrder(504);
    whenLabel->setVisible(false);
    
    addToDisplayList(whenLabel);
    
    askQuestionButton = new Button("Ask", font, "res/btn_med.png", "res/btn_med_pressed.png");
    askQuestionButton->setZOrder(504);
    askQuestionButton->setAnchorPoint(pointMake(0.5, 0.5));
    askQuestionButton->setPosition(pointMake(480, 490));
    askQuestionButton->setHandler(this);
    askQuestionButton->setVisible(false);
    askQuestionButton->setEnabled(false);
    
    addToDisplayList(askQuestionButton);
    
    cancelQuestionButton = new Button("Cancel", font, "res/btn_med.png", "res/btn_med_pressed.png");
    cancelQuestionButton->setZOrder(504);
    cancelQuestionButton->setAnchorPoint(pointMake(0.5, 0.5));
    cancelQuestionButton->setPosition(pointMake(300, 490));
    cancelQuestionButton->setHandler(this);
    cancelQuestionButton->setVisible(false);
    cancelQuestionButton->setEnabled(false);
    
    addToDisplayList(cancelQuestionButton);
    
    bkgSpeech = new Spritesheet("res/speech.png");
    bkgSpeech->setAnchorPoint(pointMake(0.35, 1));
    bkgSpeech->setCamera(camera);
    bkgSpeech->setZOrder(503);
    bkgSpeech->setVisible(false);
    
    addToDisplayList(bkgSpeech);
    
    speechLabel = new Label("speech", font, al_map_rgb(0, 0, 0), 280);
    speechLabel->setAnchorPoint(pointMake(0.5, 0.5));
    speechLabel->setCamera(camera);
    speechLabel->setZOrder(504);
    speechLabel->setVisible(false);
    
    addToDisplayList(speechLabel);
    
    speechButton = new Button(bkgSpeech->getFrameSize());
    speechButton->setAnchorPoint(bkgSpeech->getAnchorPoint());
    speechButton->setCamera(camera);
    speechButton->setZOrder(504);
    speechButton->setEnabled(false);
    speechButton->setHandler(this);
    
    addToDisplayList(speechButton);
    
    camera->setCenter(playerSprite->getPosition());
    
    activeCharacter = NULL;
    activePOI = NULL;
    currentRoom = NULL;
    
    moving = pointMake(0, 0);
    moveDir = 0;
    curFrame = 0;
    
    inputLocked = false;
    escapePressed = false;
    debug = false;
    
    currentFilter.timeStart = 0;
    currentFilter.timeEnd = QUESTION_INTERVAL;
}

bool InvestigationScene::tick(double dt) {
    
    if (inputLocked) {
        return true;
    }
    
    int tx = playerSprite->getPosition().x / collision->getTileSize().width;
    int ty = playerSprite->getPosition().y / collision->getTileSize().height;
    
    std::vector<Room *> rooms = mystery->getRooms();
    std::vector<Room *>::iterator itRooms;
    for (itRooms = rooms.begin(); itRooms < rooms.end(); ++itRooms) {
        Room *room = (Room *) *itRooms;
        
        if (rectContainsPoint(room->bounds, pointMake(tx, ty)) && room != currentRoom) {
            currentRoom = room;
            currentRoomLabel->setText(room->name.c_str());
        }
    }
    
    float dx = 0;
    float dy = 0;
    
    bool isMoving = !pointEqualsIntegral(moving, pointMake(0, 0));
    
    if (isMoving) {
        
        dx = moving.x * 200 * dt;
        dy = moving.y * 200 * dt;
        
        if (dy < 0) {
            moveDir = 0;
        } else if (dy > 0) {
            moveDir = 18;
        } else if (dx < 0) {
            moveDir = 9;
        } else if (dx > 0) {
            moveDir = 27;
        }
        
        curFrame += dt * 10;
        if (curFrame > 9) curFrame -= 9;
        
    } else {
        
        curFrame = 0;
    }
    
    bool collided = false;
    
    Rect colRect = rectMake(playerSprite->getPosition().x - 16, playerSprite->getPosition().y - 16, 32, 32);
    
    Rect colRectX = rectOffset(colRect, dx, 0);
    Rect colRectY = rectOffset(colRect, 0, dy);
    
    int tw = collision->getTileSize().width;
    int th = collision->getTileSize().height;
    
    int tpx = playerSprite->getPosition().x / collision->getTileSize().width;
    int tpy = playerSprite->getPosition().y / collision->getTileSize().height;
    
    for (int j = tpy - 2; j <= tpy + 2; j++) {
        for (int i = tpx - 2; i <= tpx + 2; i++) {
            
            if (i >= 0 && i < collision->getSize().width && j >= 0 && j < collision->getSize().height) {
                int gid = collision->getTileAt(i, j);
                
                if (gid != 0) {
                    
                    Rect tileRect = rectMake(i * tw, j * th, tw, th);
                    
                    if (rectIntersectsRect(colRectX, tileRect)) {
                        dx = 0;
                        collided = true;
                    }
                    
                    if (rectIntersectsRect(colRectY, tileRect)) {
                        dy = 0;
                        collided = true;
                    }
                }
            }
        }
    }
    
    actionButton->setVisible(false);
    actionButton->setEnabled(false);
    
    activeCharacter = NULL;
    activePOI = NULL;
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator it;
    
    for (it = characters.begin(); it < characters.end(); ++it) {
        
        Character *character = (Character *) *it;
        
        std::string action;
        
        if (character->dead) {
            action.append("Look at ");
        } else {
            action.append("Talk to ");
        }
        
        action.append(character->name);
        
        Drawable *sprite = getByTag(character->tag);
        
        Rect otherColRect = rectMake(sprite->getPosition().x - 16, sprite->getPosition().y - 16, 32, 32);
        
        if (rectIntersectsRect(colRect, otherColRect)) {
            actionButton->setLabelText(action.c_str());
            actionButton->setPosition(pointMake(sprite->getPosition().x, sprite->getPosition().y - 32));
            actionButton->setVisible(true);
            actionButton->setEnabled(true);
            
            activeCharacter = character;
        }
    }
    
    if (currentRoom != NULL) {
        
        std::vector<POI *> POIList = currentRoom->pointsOfInterest;
        std::vector<POI *>::iterator itPOI;
        
        for (itPOI = POIList.begin(); itPOI < POIList.end(); ++itPOI) {
            
            POI *poi = (POI *) *itPOI;
            
            Rect tileRect = collision->getTileRect(poi->position.x, poi->position.y);
            tileRect = rectExpand(tileRect, 4, 4);
            
            if ((poi->interest == InterestContainerVisible || poi->interest == InterestContainerConceiled) && rectIntersectsRect(colRect, tileRect)) {
                
                std::string action = "Examine ";
                action.append(poi->shortDescription);
                
                actionButton->setLabelText(action.c_str());
                actionButton->setPosition(rectMidPoint(tileRect));
                actionButton->setVisible(true);
                actionButton->setEnabled(true);
                
                activePOI = poi;
            }
        }
    }
    
    playerSprite->setFrame(moveDir + (int)curFrame);
    playerSprite->setPosition(pointOffset(playerSprite->getPosition(), dx, dy));
    
    camera->setCenter(playerSprite->getPosition());
    
    return !escapePressed;
}

void InvestigationScene::draw() {
    Scene::draw();
    
    if (debug) {
        
        al_draw_rectangle(playerSprite->getPosition().x - 16 - camera->getTop().x, playerSprite->getPosition().y - 16 - camera->getTop().y, playerSprite->getPosition().x + 16 - camera->getTop().x, playerSprite->getPosition().y + 16 - camera->getTop().y, al_map_rgb(255, 0, 0), 1);
        
        int tw = collision->getTileSize().width;
        int th = collision->getTileSize().height;
        
        int tpx = playerSprite->getPosition().x / collision->getTileSize().width;
        int tpy = playerSprite->getPosition().y / collision->getTileSize().height;
        
        for (int j = tpy - 2; j <= tpy + 2; j++) {
            for (int i = tpx - 2; i <= tpx + 2; i++) {
                
                if (i >= 0 && i < collision->getSize().width && j >= 0 && j < collision->getSize().height) {
                    int gid = collision->getTileAt(i, j);
                    
                    if (gid != 0) {
                        
                        al_draw_rectangle(i * tw - camera->getTop().x, j * th - camera->getTop().y, i * tw + tw - camera->getTop().x, j * th + th - camera->getTop().y, al_map_rgb(255, 0, 0), 1);
                    }
                }
            }
        }
    }
}

void InvestigationScene::onKeyDown(int keycode, ALLEGRO_EVENT ev) {
    
    if (inputLocked) {
        return;
    }
    
    switch (keycode) {
        case ALLEGRO_KEY_UP:
            moving = pointOffset(moving, 0, -1);
            break;
        case ALLEGRO_KEY_DOWN:
            moving = pointOffset(moving, 0, 1);
            break;
        case ALLEGRO_KEY_LEFT:
            moving = pointOffset(moving, -1, 0);
            break;
        case ALLEGRO_KEY_RIGHT:
            moving = pointOffset(moving, 1, 0);
            break;
        default:
            break;
    }
}

void InvestigationScene::onKeyUp(int keycode, ALLEGRO_EVENT ev) {
    
    if (inputLocked) {
        return;
    }
    
    switch (keycode) {
        case ALLEGRO_KEY_UP:
            moving = pointOffset(moving, 0, 1);
            break;
        case ALLEGRO_KEY_DOWN:
            moving = pointOffset(moving, 0, -1);
            break;
        case ALLEGRO_KEY_LEFT:
            moving = pointOffset(moving, 1, 0);
            break;
        case ALLEGRO_KEY_RIGHT:
            moving = pointOffset(moving, -1, 0);
            break;
        case ALLEGRO_KEY_ESCAPE:
            escapePressed = true;
            break;
        default:
            break;
    }
}

void InvestigationScene::onButtonClicked(Button *sender) {
    
    if (sender == actionButton) {
        
        if (activeCharacter != NULL) {
            
            printf("Talked to %s\n", activeCharacter->name.c_str());
            questionStart();
            
        } else if (activePOI != NULL) {
            
            printf("Interacted with %s\n", activePOI->description.c_str());
        }
    
    } else if (sender == cancelQuestionButton) {
        
        questionEnd();
        
        inputLocked = false;
    
    } else if (sender == askQuestionButton) {
            
        questionEnd();
        
        dialogueStart();
    
    } else if (sender == speechButton) {
        
        if (speechIdx < speechLines.size()) {
            
            dialogueAdvance();
            
        } else {
        
            dialogueEnd();
        
            inputLocked = false;
        }
        
    } else if (sender->getTag() > 100 && sender->getTag() < 200) {
        
        switch (sender->getTag()) {
            case 101:
                questionWho();
                break;
            case 102:
                questionWhere();
            default:
                break;
        }
        
    } else if (sender->getTag() > 200 && sender->getTag() < 300) {
        
        currentFilter.where = (Room *) sender->data;
        
        questionWhen();
        
    } else if (sender->getTag() > 300 && sender->getTag() < 400) {
        
        currentFilter.who = (Character *) sender->data;
        
        questionWhen();
        
    } else if (sender->getTag() > 400 && sender->getTag() < 500) {
        
        int amnt = 0;
        
        switch (sender->getTag()) {
            case 401:
                amnt = -QUESTION_INTERVAL;
                break;
            case 402:
                amnt = QUESTION_INTERVAL;
            default:
                break;
        }
        
        if ((currentFilter.timeStart + amnt) < 0 || (currentFilter.timeEnd + amnt) > MAX_TIME) {
            amnt = 0;
        }
        
        currentFilter.timeStart += amnt;
        currentFilter.timeEnd += amnt;
        
        questionWhen();
        
    }
}

void InvestigationScene::removeQuestionElements() {
    
    std::vector<Drawable *>::iterator it;
    
    for (it = questionElements.begin(); it < questionElements.end(); ++it) {
        Drawable *drawable = (Drawable *) *it;
        removeFromDisplayList(drawable, true);
    }
    
    questionElements.clear();
}

void InvestigationScene::questionStart() {
    
    inputLocked = true;
    
    currentFilter.where = NULL;
    currentFilter.who = NULL;
    
    actionButton->setEnabled(false);
    actionButton->setVisible(false);
    
    cancelQuestionButton->setEnabled(true);
    cancelQuestionButton->setVisible(true);
    
    removeQuestionElements();
    
    std::string question = "Select a question to ask ";
    question.append(activeCharacter->name);
    
    questionLabel->setText(question.c_str());
    questionLabel->setVisible(true);
    
    bkgQuestion->setVisible(true);
    
    Button *button = new Button("Who was in the...", font, "res/btn_action.png", "res/btn_action_pressed.png");
    button->setZOrder(504);
    button->setAnchorPoint(pointMake(0.5, 0.5));
    button->setPosition(pointMake(400, 200));
    button->setTag(101);
    button->setHandler(this);
    
    addToDisplayList(button);
    questionElements.push_back(button);
    
    button = new Button("Where was...", font, "res/btn_action.png", "res/btn_action_pressed.png");
    button->setZOrder(504);
    button->setAnchorPoint(pointMake(0.5, 0.5));
    button->setPosition(pointMake(400, 300));
    button->setTag(102);
    button->setHandler(this);
    
    addToDisplayList(button);
    questionElements.push_back(button);
}

void InvestigationScene::questionWho() {
    
    removeQuestionElements();
    
    questionLabel->setText("Who was in the ... ?");
    
    std::vector<Room *> rooms = mystery->getRooms();
    std::vector<Room *>::iterator it;
    
    int i = 0;
    
    int px = 300;
    int py = 200;
    
    for (it = rooms.begin(); it < rooms.end(); ++it) {
        
        Room *room = (Room *) *it;
        
        Button *button = new Button(room->name.c_str(), font, "res/btn_med.png", "res/btn_med_pressed.png");
        button->setZOrder(504);
        button->setAnchorPoint(pointMake(0.5, 0.5));
        button->setPosition(pointMake(px, py));
        button->setTag(201 + i);
        button->data = room;
        button->setHandler(this);
        
        addToDisplayList(button);
        questionElements.push_back(button);
        
        py += 36;
        
        if (i == 6) {
            py = 200;
            px += 192;
        }
        
        i++;
    }
}

void InvestigationScene::questionWhere() {
    
    removeQuestionElements();
    
    questionLabel->setText("Where was ... ?");
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator it;
    
    int i = 0;
    
    int px = 400;
    int py = 200;
    
    for (it = characters.begin(); it < characters.end(); ++it) {
        
        Character *character = (Character *) *it;
        
        Button *button = new Button(character->name.c_str(), font, "res/btn_action.png", "res/btn_action_pressed.png");
        button->setZOrder(504);
        button->setAnchorPoint(pointMake(0.5, 0.5));
        button->setPosition(pointMake(px, py));
        button->setTag(301 + i);
        button->data = character;
        button->setHandler(this);
        
        addToDisplayList(button);
        questionElements.push_back(button);
        
        py += 36;
        
        i++;
    }
}

void InvestigationScene::questionWhen() {
    
    removeQuestionElements();
    
    std::string question;
    
    if (currentFilter.where == NULL) {
        
        question = "Where was ";
        question.append(currentFilter.who->name);
        
    } else if (currentFilter.who == NULL) {
        
        question = "Who was in the ";
        question.append(currentFilter.where->name);
    }
    
    question.append(" between ... ?");
    
    questionLabel->setText(question.c_str());
    
    Button *button = new Button("<", fontBig, "res/btn_small.png", "res/btn_small_pressed.png");
    button->setZOrder(504);
    button->setAnchorPoint(pointMake(0.5, 0.5));
    button->setPosition(pointMake(260, 300));
    button->setTag(401);
    button->setHandler(this);
    
    addToDisplayList(button);
    questionElements.push_back(button);
    
    button = new Button(">", fontBig, "res/btn_small.png", "res/btn_small_pressed.png");
    button->setZOrder(504);
    button->setAnchorPoint(pointMake(0.5, 0.5));
    button->setPosition(pointMake(540, 300));
    button->setTag(402);
    button->setHandler(this);
    
    addToDisplayList(button);
    questionElements.push_back(button);
    
    char buff[20];
    sprintf(buff, "%s and %s", timeToString(currentFilter.timeStart + START_TIME, false).c_str(), timeToString(currentFilter.timeEnd + START_TIME, false).c_str());
    
    whenLabel->setText(buff);
    whenLabel->setVisible(true);
    
    askQuestionButton->setEnabled(true);
    askQuestionButton->setVisible(true);
}

void InvestigationScene::questionEnd() {
    
    removeQuestionElements();
    
    askQuestionButton->setEnabled(false);
    askQuestionButton->setVisible(false);
    
    cancelQuestionButton->setEnabled(false);
    cancelQuestionButton->setVisible(false);
    
    bkgQuestion->setVisible(false);
    
    questionLabel->setVisible(false);
    
    whenLabel->setVisible(false);
}

void InvestigationScene::dialogueStart() {
    
    speechLines.clear();
    
    speechLines.push_back(std::string("what"));
    speechLines.push_back(std::string("about"));
    speechLines.push_back(std::string("it?"));
    
    speechIdx = 0;
    
    Drawable *sprite = getByTag(activeCharacter->tag);
    
    bkgSpeech->setVisible(true);
    bkgSpeech->setPosition(pointMake(sprite->getPosition().x, sprite->getPosition().y - 20));
    
    speechLabel->setText(speechLines[speechIdx].c_str());
    speechLabel->setVisible(true);
    speechLabel->setPosition(pointOffset(bkgSpeech->getPosition(), 60, -175));
    
    speechButton->setEnabled(true);
    speechButton->setPosition(bkgSpeech->getPosition());
    
    speechIdx++;
}

void InvestigationScene::dialogueAdvance() {
    
    speechLabel->setText(speechLines[speechIdx].c_str());
    
    speechIdx++;
}

void InvestigationScene::dialogueEnd() {
    
    bkgSpeech->setVisible(false);
    speechLabel->setVisible(false);
    speechButton->setEnabled(false);
    
}
