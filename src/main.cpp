#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

CCMenu* menu;
LoadingCircle* loading;
CCNode* l;
std::vector<CCNode*> buttons;

class $modify(LevelInfoLayer)
{
	bool init(GJGameLevel* p0, bool p1)
	{
		buttons.clear();

		if (!LevelInfoLayer::init(p0, p1))
			return false;

		menu = CCMenu::create();
		menu->setContentSize(ccp(0, 0));
		menu->setID("tags-menu"_spr);
		menu->setScale(0.15f);
		menu->setPosition(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getPosition() + ccp(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getScaledContentSize().width / 2 + 7 + (this->getChildByID("copy-indicator") ? 18 : 0) + (this->getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));

		loading = LoadingCircle::create();
		loading->ignoreAnchorPointForPosition(false);
		loading->m_fade = false;
		loading->show();
		menu->addChild(loading);

		auto ship = CCSprite::createWithSpriteFrameName("portal_04_extra_2_001.png");
		ship->setScale(3.5f);

		l = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large); // CCSprite::createWithSpriteFrameName("GJ_dislikesIcon_001.png");
		l->setID("legacy-ship");
		buttons.push_back(l);
		menu->addChild(l);

		if (p0->m_twoPlayerMode)
		{
			auto cube = CCSprite::createWithSpriteFrameName("portal_03_extra_2_001.png");
			//auto cube = CCSprite::createWithSpriteFrameName("GJ_dislikesIcon_001.png");
			cube->setScale(3.5f);

			auto p2 = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
			p2->setID("2-player");

			menu->addChild(p2);
			
			buttons.push_back(p2);
		}

		this->addChild(menu);

		if (p0->m_levelString.size() > 1)
		{
			auto pl = PlayLayer::create(p0, true, true);
			CCDirector::get()->getOpenGLView()->showCursor(true);
			delete pl;

			l->setVisible(!pl->m_levelSettings->m_propertykA32);

			loading->removeFromParent();
			if (!l->isVisible())
			{
				l->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), l), buttons.end());
			}
		}
		else
		{
			buttons.push_back(l);
			
			l->setVisible(false);
			loading->setVisible(true);
		}

		for (size_t i = 0; i < buttons.size(); i++)
		{
			buttons[i]->setPositionX((18 / 0.15f) * i);
		}
		

		return true;
	}

	virtual TodoReturn levelDownloadFinished(GJGameLevel* p0)
	{
		LevelInfoLayer::levelDownloadFinished(p0);

		log::info("levelDownloadFinished");

		if (menu)
		{
			if (p0->m_levelString.size() > 1)
			{
				auto pl = PlayLayer::create(p0, true, true);
				CCDirector::get()->getOpenGLView()->showCursor(true);
				delete pl;

				l->setVisible(!pl->m_levelSettings->m_propertykA32);
				loading->setVisible(false);

				if (!l->isVisible())
				{
					l->removeFromParent();
					buttons.erase(std::remove(buttons.begin(), buttons.end(), l), buttons.end());
				}
			}
			else
			{
				l->setVisible(false);
				loading->setVisible(true);
			}

			for (size_t i = 0; i < buttons.size(); i++)
			{
				buttons[i]->setPositionX((18 / 0.15f) * i);
			}

		}
	}
};