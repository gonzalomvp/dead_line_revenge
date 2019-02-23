#pragma once

class IScene {
public:
	enum EScene {
		EMENU,
		EGAME,
		EINVALID,
	};

	virtual ~IScene() {}

	virtual void   init         ()                      = 0;
	virtual void   processInput ()                const = 0;
	virtual void   run          (float deltaTime) const = 0;
	virtual void   render       ()                const = 0;
	virtual EScene getSceneType ()                const = 0;
};
