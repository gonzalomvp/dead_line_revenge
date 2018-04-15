#include "gui.h"
#include "../globals.h"
#include "../engine/graphics_engine.h"
#include "menu_item.h"

GUIManager* GUIManager::mInstance;


GUIManager::GUIManager() {
	
}


void GUIManager::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	//for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
	//	(*itControls)->render();
	//}
	FONT_DrawString(vmake(0,0), "TO DO IN-GAME MENU");
	SYS_Show();
}

Control::~Control() {
	g_inputManager->unregisterEvent(this);
}

Button::Button(std::string name, vec2 pos, vec2 size, const char* spriteOn, const char* spriteOff) : Control(name, pos, size) {
	m_spriteOn = new Sprite(g_graphicsEngine->getTexture(spriteOn), 1);
	m_spriteOn->setPos(pos);
	m_spriteOn->setSize(size);
	m_spriteOff = new Sprite(g_graphicsEngine->getTexture(spriteOff), 1);
	m_spriteOff->setPos(pos);
	m_spriteOff->setSize(size);
	g_graphicsEngine->addGfxEntity(m_spriteOff);
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
	m_pressed = false;
}

bool Button::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EMouse) {
		const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);

		if (mouseEvent.mouseButton == IInputManager::MouseEvent::BLeft && mouseEvent.mouseButtonAction == IInputManager::MouseEvent::AButtonDown
			&& mouseEvent.x >= m_pos.x - m_size.x * 0.5 && mouseEvent.x <= m_pos.x + m_size.x * 0.5 && mouseEvent.y >= m_pos.y - m_size.y * 0.5 && mouseEvent.y <= m_pos.y + m_size.y * 0.5) {
			m_pressed = true;
			g_graphicsEngine->removeGfxEntity(m_spriteOff);
			g_graphicsEngine->addGfxEntity(m_spriteOn);
		}

		if (mouseEvent.mouseButton == IInputManager::MouseEvent::BLeft && mouseEvent.mouseButtonAction == IInputManager::MouseEvent::AButtonUp) {
			if (m_pressed && mouseEvent.x >= m_pos.x - m_size.x * 0.5 && mouseEvent.x <= m_pos.x + m_size.x * 0.5 && mouseEvent.y >= m_pos.y - m_size.y * 0.5 && mouseEvent.y <= m_pos.y + m_size.y * 0.5) {
				for (size_t i = 0; i < m_listeners.size(); ++i) {
					m_listeners[i]->onClick(this);
					//temp
					m_listeners.clear();
				}
			}
			m_pressed = false;
			g_graphicsEngine->removeGfxEntity(m_spriteOn);
			g_graphicsEngine->addGfxEntity(m_spriteOff);
		}
	}

	return true;
}