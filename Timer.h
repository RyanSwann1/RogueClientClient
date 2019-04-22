#pragma once

class Timer
{
public:
	Timer(float expirationTime);

	bool isExpired() const;
	void update(float deltaTime);
	void reset();

private:
	const float m_expirationTime;
	float m_elaspedTime;
};