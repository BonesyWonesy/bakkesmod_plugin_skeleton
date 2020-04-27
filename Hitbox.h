#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <vector>
#include "DebugLog.h"

class Hitbox
{
public:
		Hitbox();
		void getPoints(std::vector<Vector>& pts);
		~Hitbox();

		//void UseHitBoxForCar(CARBODY carType);


		void OnLoad(DebugLog* debugLogger);
		void OnDestroy();
		void Render(CanvasWrapper& canvas, std::shared_ptr<GameWrapper> gameWrapper);

private:
		Vector Rotate(Vector aVec, double roll, double yaw, double pitch);
		void computePoints(float length, float width, float height, float x_offset, float y_offset, float z_offset);
		std::vector<Vector> m_hitboxPoints;

		DebugLog* m_debugLog;
		bool m_loaded;
};

