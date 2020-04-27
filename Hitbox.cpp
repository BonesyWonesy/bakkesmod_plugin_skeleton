#include "Hitbox.h"


Hitbox::Hitbox() : m_loaded(false) {
}


Hitbox::~Hitbox() {
}

void Hitbox::OnLoad(DebugLog* debugLogger) {
	if (m_loaded) {
		return;
	}
	m_debugLog = debugLogger;
}

void Hitbox::OnDestroy() {

}

void Hitbox::computePoints(float length, float width, float height, float x_offset, float y_offset, float z_offset) {
	m_hitboxPoints.clear();
	m_hitboxPoints.push_back(Vector(length + x_offset, height + z_offset, width + y_offset));
	m_hitboxPoints.push_back(Vector(length + x_offset, height + z_offset, -width + y_offset));
	m_hitboxPoints.push_back(Vector(-length + x_offset, height + z_offset, -width + y_offset));
	m_hitboxPoints.push_back(Vector(-length + x_offset, height + z_offset, width + y_offset));
	m_hitboxPoints.push_back(Vector(length + x_offset, -height + z_offset, width + y_offset));
	m_hitboxPoints.push_back(Vector(length + x_offset, -height + z_offset, -width + y_offset));
	m_hitboxPoints.push_back(Vector(-length + x_offset, -height + z_offset, -width + y_offset));
	m_hitboxPoints.push_back(Vector(-length + x_offset, -height + z_offset, width + y_offset));
}

void Hitbox::getPoints(std::vector<Vector>& pts)
{
	pts.clear();
	for (int i = 0; i < 8; i++)
		pts.push_back(m_hitboxPoints[i]);
}

Vector Hitbox::Rotate(Vector aVec, double roll, double yaw, double pitch)
{
	double sx = sin(roll);
	double cx = cos(roll);
	double sy = sin(yaw);
	double cy = cos(yaw);
	double sz = sin(pitch);
	double cz = cos(pitch);

	aVec = Vector(aVec.X, aVec.Y * cx - aVec.Z * sx, aVec.Y * sx + aVec.Z * cx);  //2  roll? 
	aVec = Vector(aVec.X * cz - aVec.Y * sz, aVec.X * sz + aVec.Y * cz, aVec.Z); //1   pitch?
	aVec = Vector(aVec.X * cy + aVec.Z * sy, aVec.Y, -aVec.X * sy + aVec.Z * cy);  //3  yaw?

  // ugly fix to change coordinates to Unreal's axes
	float tmp = aVec.Z;
	aVec.Z = aVec.Y;
	aVec.Y = tmp;
	return aVec;
}



void Hitbox::Render(CanvasWrapper& canvas, std::shared_ptr<GameWrapper> gameWrapper) {
	if (!gameWrapper->IsInGame()) {
		if ( m_debugLog ) {
			m_debugLog->Log("Not in a game, skipping hitbox render");
		}
		return;
	}

	ServerWrapper gameServer = gameWrapper->GetGameEventAsServer();
		
	if (gameServer.IsNull()) {
		if (m_debugLog) {
			m_debugLog->Log("gameServer is null");
		}
		return;
	}

	VehicleWrapper gameCar = (VehicleWrapper)gameServer.GetGameCar();
	if (gameCar.IsNull()) {
		if (m_debugLog) {
			m_debugLog->Log("gameCar is null");
		}
		return;
	}

	// Compute the hitbox for the car
	if (m_hitboxPoints.size() == 0 ) {
		Vector extent = gameCar.GetLocalCollisionExtent();
		Vector offset = gameCar.GetLocalCollisionOffset();

		computePoints(extent.X, extent.Y, extent.Z, offset.X, offset.Y, offset.Z);
	}

	canvas.SetColor(255, 255, 0, 200);

	Vector v = gameCar.GetLocation();
	Rotator r = gameCar.GetRotation();


	double dPitch = (double)r.Pitch / 32764.0 * 3.14159;
	double dYaw = (double)r.Yaw / 32764.0 * 3.14159;
	double dRoll = (double)r.Roll / 32764.0 * 3.14159;

	Vector2 carLocation2D = canvas.Project(v);
	Vector2 hitbox2D[8];
	for (int i = 0; i < 8; i++) {
			hitbox2D[i] = canvas.Project(Rotate(m_hitboxPoints[i], dRoll, -dYaw, dPitch) + v);
	}

	canvas.DrawLine(hitbox2D[0], hitbox2D[1]);
	canvas.DrawLine(hitbox2D[1], hitbox2D[2]);
	canvas.DrawLine(hitbox2D[2], hitbox2D[3]);
	canvas.DrawLine(hitbox2D[3], hitbox2D[0]);
	canvas.DrawLine(hitbox2D[4], hitbox2D[5]);
	canvas.DrawLine(hitbox2D[5], hitbox2D[6]);
	canvas.DrawLine(hitbox2D[6], hitbox2D[7]);
	canvas.DrawLine(hitbox2D[7], hitbox2D[4]);
	canvas.DrawLine(hitbox2D[0], hitbox2D[4]);
	canvas.DrawLine(hitbox2D[1], hitbox2D[5]);
	canvas.DrawLine(hitbox2D[2], hitbox2D[6]);
	canvas.DrawLine(hitbox2D[3], hitbox2D[7]);

	canvas.SetPosition(carLocation2D.minus((Vector2{ 10,10 })));
	canvas.FillBox((Vector2{ 20, 20 }));
}