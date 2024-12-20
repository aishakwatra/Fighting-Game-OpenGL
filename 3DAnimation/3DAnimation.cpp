#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "animator.h"
#include "model_animation.h"
#include "Player.h"

#include <iostream>

<<<<<<< Updated upstream
=======
#define MAX_HEALTH 150.0f

enum AnimStateP1 {
	P1_IDLE = 1,
	P1_IDLE_PUNCH,
	P1_PUNCH_IDLE,
	P1_IDLE_KICK,
	P1_KICK_IDLE,
	P1_IDLE_BLOCK,
	P1_BLOCK_IDLE,
	P1_IDLE_WALK_FRONT,
	P1_IDLE_WALK_BACK,
	P1_WALK_FRONT_IDLE,
	P1_WALK_BACK_IDLE,
	P1_WALK_FRONT,
	P1_WALK_BACK,
	P1_IDLE_HIT,
	P1_HIT_IDLE
	
};

enum AnimStateP2 {
	P2_IDLE = 1,
	P2_IDLE_PUNCH,
	P2_PUNCH_IDLE,
	P2_IDLE_KICK,
	P2_KICK_IDLE,
	P2_IDLE_BLOCK,
	P2_BLOCK_IDLE,
	P2_IDLE_WALK_FRONT,
	P2_IDLE_WALK_BACK,
	P2_WALK_FRONT_IDLE,
	P2_WALK_BACK_IDLE,
	P2_WALK_FRONT,
	P2_WALK_BACK
};

enum GameState {
	INTRO_P1,
	INTRO_P2,
	COUNTDOWN,
	GAMEPLAY,
	END_ROUND,
	RESTART
};

GameState currentState = INTRO_P1;

bool gameStart = false;
float introTimer = 0.0f;
float countdown = 3.0f;  // 3 seconds countdown


struct Capsule {
	glm::vec3 pointA;  // Top center of the capsule
	glm::vec3 pointB;  // Bottom center of the capsule
	float radius;
};

Capsule player1Capsule;
Capsule player2Capsule;

int player1Health = MAX_HEALTH;
int player2Health = MAX_HEALTH;

CountdownTimer timer;
>>>>>>> Stashed changes

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

<<<<<<< Updated upstream
std::map<std::string, std::string> player1Anims = {
	{"idle", "Object/Vegas/Idle.dae"},
	{"walk", "Object/Vegas/WalkForward.dae"},
	{"run", "Object/Vegas/WalkBack.dae"},
	{"punch", "Object/Vegas/Punching.dae"},
	{"kick", "Object/Vegas/Kick.dae"}
};
=======
ModelAnim player1;
Animation introAnimationP1;
Animation idleAnimationP1;
Animation walkFrontAnimationP1;
Animation walkBackAnimationP1;
Animation punchAnimationP1;
Animation kickAnimationP1;
Animation blockAnimationP1;
Animation hitAnimationP1;

ModelAnim player2;
Animation introAnimationP2;
Animation idleAnimationP2;
Animation walkFrontAnimationP2;
Animation walkBackAnimationP2;
Animation punchAnimationP2;
Animation kickAnimationP2;
Animation blockAnimationP2;
Animation hitAnimationP2;

float P1animStateTimer = 0.0f;

glm::vec3 player1Position = glm::vec3(0.0f, -0.4f, -2.0f);
glm::vec3 player2Position = glm::vec3(0.0f, -0.4f, 5.0f);
glm::vec3 player1gamePosition = glm::vec3(0.0f, -0.4f, 0.0f);
glm::vec3 player2gamePosition = glm::vec3(0.0f, -0.4f, 3.0f);
float moveSpeed = 0.5f;
int P1punchDamage = 1;
int P1KickDamage = 2;
int P2punchDamage = 3;
int P2KickDamage = 4;
Animator player1_animator(&idleAnimationP1);
Animator player2_animator(&idleAnimationP2);
enum AnimStateP1 P1charState = P1_IDLE;
enum AnimStateP2 P2charState = P2_IDLE;
float blendAmountP1 = 0.0f;
float blendAmountP2 = 0.0f;
float blendRate = 0.055f;

float lerp(float start, float end, float t) {
	return start + t * (end - start);
}



void setupIntro() {
	// Only start Player 1's intro animation initially
	player1_animator.PlayAnimation(&introAnimationP1, nullptr, 0.0f, 0.0f, 0.0f);
}


void updateIntro(GLFWwindow* window, float deltaTime) {
	float introDurationP1 = introAnimationP1.GetDuration() / 1000.0f; // Convert ms to seconds
	float introDurationP2 = introAnimationP2.GetDuration() / 1000.0f; // Convert ms to seconds
	static float transitionDuration = 1.0f; // Duration of the camera transition
	static float transitionTimer = 0.0f; // Timer for the camera transition

	if (currentState == INTRO_P1) {
		introTimer += deltaTime;
		if (introTimer >= introDurationP2 || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			currentState = INTRO_P2;
			introTimer = 0.0f;
			transitionTimer = 0.0f; // Reset transition timer
			player1_animator.PlayAnimation(&idleAnimationP1, nullptr, 0.0f, 0.0f, 0.0f);
			player2_animator.PlayAnimation(&introAnimationP2, nullptr, 0.0f, 0.0f, 0.0f);
		}
	}
	else if (currentState == INTRO_P2) {
		if (transitionTimer < transitionDuration) {
			// Perform the interpolation
			float t = transitionTimer / transitionDuration;
			camera.Yaw = lerp(introP1camYaw, introP2camYaw, t);
			camera.Pitch = lerp(introP1camPitch, introP2camPitch, t);
			camera.updateCameraVectors();
			transitionTimer += deltaTime;
		}
		else {
			// Ensure the final values are set after interpolation
			camera.Yaw = introP2camYaw;
			camera.Pitch = introP2camPitch;
			camera.updateCameraVectors();
		}

		introTimer += deltaTime;
		if (introTimer >= introDurationP2 || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // End intro slightly earlier to blend to countdown
			// Reset positions and camera after intro
			player2Position = player2gamePosition; // Reset to game position
			player1Position = player1gamePosition;
			camera.Yaw = gameCamYaw;
			camera.Pitch = gameCamPitch;
			camera.Position = gameCamPos;
			camera.updateCameraVectors();
			player1_animator.PlayAnimation(&idleAnimationP1, nullptr, 0.0f, 0.0f, 0.0f);
			player2_animator.PlayAnimation(&idleAnimationP2, nullptr, 0.0f, 0.0f, 0.0f);
			currentState = COUNTDOWN;
			introTimer = 0.0f;
		}
	}
	
}



void startCountdown(float deltaTime) {
	countdown -= deltaTime;
	if (countdown <= 0.0f) {
		gameStart = true;
		currentState = GAMEPLAY;

		
		timer.resetToDefault();
		timer.start();

		// Ensure both players are set to idle at the start of gameplay
		P1charState = P1_IDLE;
		P2charState = P2_IDLE;

		// Start idle animations for both players
		player1_animator.PlayAnimation(&idleAnimationP1, nullptr, 0.0f, 0.0f, 0.0f);
		player2_animator.PlayAnimation(&idleAnimationP2, nullptr, 0.0f, 0.0f, 0.0f);

		// Reset camera position if necessary
		camera.Position = gameCamPos;  // Example camera position
	}
}

void updateCapsules() {
	// Adjust these values based on the character's current pose and animation
	player1Capsule.pointA = player1Position + glm::vec3(0.0f, 1.2f, 0.0f); // example values
	player1Capsule.pointB = player1Position + glm::vec3(0.0f, 0.4f, 0.0f);
	player1Capsule.radius = 0.4f;

	player2Capsule.pointA = player2Position + glm::vec3(0.0f, 1.2f, 0.0f);
	player2Capsule.pointB = player2Position + glm::vec3(0.0f, 0.4f, 0.0f);
	player2Capsule.radius = 0.4f;
}

float segmentSegmentDistance(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& q1, const glm::vec3& q2) {
	glm::vec3 u = p2 - p1;
	glm::vec3 v = q2 - q1;
	glm::vec3 w = p1 - q1;
	float a = glm::dot(u, u); // always >= 0
	float b = glm::dot(u, v);
	float c = glm::dot(v, v); // always >= 0
	float d = glm::dot(u, w);
	float e = glm::dot(v, w);
	float D = a * c - b * b; // always >= 0
	float sc, sN, sD = D;    // sc = sN / sD, default sD = D >= 0
	float tc, tN, tD = D;    // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < 1e-8) { // the lines are almost parallel
		sN = 0.0; // force using point P0 on segment S1
		sD = 1.0; // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else { // get the closest points on the infinite lines
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) { // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) { // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) { // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) { // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < 1e-8 ? 0.0 : sN / sD);
	tc = (abs(tN) < 1e-8 ? 0.0 : tN / tD);

	// get the difference of the two closest points
	glm::vec3 dP = w + (sc * u) - (tc * v); // =  S1(sc) - S2(tc)

	return glm::length(dP); // return the closest distance
}

bool checkCapsuleCollision(const Capsule& capsule1, const Capsule& capsule2) {
	float distance = segmentSegmentDistance(capsule1.pointA, capsule1.pointB, capsule2.pointA, capsule2.pointB);
	return distance < (capsule1.radius + capsule2.radius);
}

void handleCollisions() {
	if (checkCapsuleCollision(player1Capsule, player2Capsule)) {
		glm::vec3 collisionNormal = glm::normalize(player2Position - player1Position);
		float overlap = (player1Capsule.radius + player2Capsule.radius) - glm::distance(player1Capsule.pointB, player2Capsule.pointB);
		float pushFactor = 0.1f;

		// Push both players away from each other
		player1Position -= collisionNormal * (overlap * pushFactor) * 0.5f;
		player2Position += collisionNormal * (overlap * pushFactor) * 0.5f;

		// Handling for Player 1 attacking
		if (P1charState == P1_PUNCH_IDLE || P1charState == P1_KICK_IDLE) {
			int damage = 0;
			if (P1charState == P1_PUNCH_IDLE) {
				damage = P1punchDamage;
			}
			else if (P1charState == P1_KICK_IDLE) {
				damage = P1KickDamage;
			}

			// Check if Player 2 is currently blocking
			if (P2charState == P2_IDLE_BLOCK) {
				std::cout << "Player 2 blocked the attack!" << std::endl;
			}
			else {
				// Apply damage to Player 2 if not blocking
				player2Health -= damage;
				std::cout << "Player 2 hit! Health now: " << player2Health << std::endl;
				if (player2Health <= 0) {
					std::cout << "Player 2 has been defeated!" << std::endl;
				}
			}
		}

		// Handling for Player 2 attacking
		if (P2charState == P2_PUNCH_IDLE || P2charState == P2_KICK_IDLE) {
			float animationTime = player2_animator.getCurrentAnimationTime(); // Assuming you have a method to get current time
			std::cout << "Player 2 animation time: " << animationTime << std::endl;
			int damage = punchAnimationP2.getDamageForTime(animationTime); // Adjust to use animation-specific damage checks

			if (damage > 0) { // Damage is applied only if the current frame is a damage keyframe
				// Check if Player 1 is currently blocking
				if (P1charState == P1_IDLE_BLOCK) {
					std::cout << "Player 1 blocked the attack!" << std::endl;
				}
				else {
					// Apply damage to Player 1 if not blocking
					player1Health -= damage;
					std::cout << "Player 1 hit! Health now: " << player1Health << std::endl;
					P1charState = P1_IDLE_HIT; // Update the state to reflect being hit
					if (player1Health <= 0) {
						std::cout << "Player 1 has been defeated!" << std::endl;
					}
				}
			}
		}

	}
}

void RenderHealthBars(Shader& shader, unsigned int texture) {

	// Maximum width of the health bar
	const float maxBarWidth = 400.0f;

	// Calculate the dynamic width based on the health ratio
	float player1HealthRatio = (float)player1Health / MAX_HEALTH; 
	float player2HealthRatio = (float)player2Health / MAX_HEALTH;

	float player1BarWidth = maxBarWidth * player1HealthRatio;
	float player2BarWidth = maxBarWidth * player2HealthRatio;

	// Adjust the position for Player 1's health bar
	float player1BarX = 50.0f; // Left side stays fixed

	// Adjust the position for Player 2's health bar
	float player2BarX = SCR_WIDTH - (player1BarX + maxBarWidth) + (maxBarWidth - player2BarWidth); // Right side stays fixed

	// Render Player 1's health bar
	RenderUIElement(shader, texture, player1BarX, SCR_HEIGHT - 100.0f, player1BarWidth, 40.0f);

	// Render Player 2's health bar
	RenderUIElement(shader, texture, player2BarX, SCR_HEIGHT - 100.0f, player2BarWidth, 40.0f);

}

void UpdateStateP1(GLFWwindow* window, Animator& animator, AnimStateP1& charState, float& blendAmount)
{
	switch (charState) {
	case P1_IDLE:
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			blendAmount = 0.0f;
			player1Position.z += moveSpeed * deltaTime;
			animator.PlayAnimation(&idleAnimationP1, &walkFrontAnimationP1, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P1_IDLE_WALK_FRONT;
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			blendAmount = 0.0f;
			player1Position.z -= moveSpeed * deltaTime;
			animator.PlayAnimation(&idleAnimationP1, &walkBackAnimationP1, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P1_IDLE_WALK_BACK;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP1, &blockAnimationP1, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P1_IDLE_BLOCK;
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP1, &punchAnimationP1, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P1_IDLE_PUNCH;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP1, &kickAnimationP1, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P1_IDLE_KICK;
		}
		//printf("idle \n");
		break;
	case P1_IDLE_WALK_FRONT:
		player1Position.z += moveSpeed * deltaTime;
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &walkFrontAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&walkFrontAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_WALK_FRONT;
		}
		//printf("idle_walk_front \n");
		break;
	case P1_IDLE_WALK_BACK:
		player1Position.z -= moveSpeed * deltaTime;
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &walkBackAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&walkBackAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_WALK_BACK;
		}
		//printf("idle_walk_back \n");
		break;
	case P1_WALK_FRONT:
		player1Position.z += moveSpeed * deltaTime;
		animator.PlayAnimation(&walkFrontAnimationP1, NULL, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
			charState = P1_WALK_FRONT_IDLE;

		}
		//printf("walking_front\n");
		break;
	case P1_WALK_BACK:
		player1Position.z -= moveSpeed * deltaTime;
		animator.PlayAnimation(&walkBackAnimationP1, NULL, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS) {
			charState = P1_WALK_BACK_IDLE;

		}
		//printf("walking_back\n");
		break;
	case P1_WALK_FRONT_IDLE:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&walkFrontAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_IDLE;
		}
		//printf("walk_front_idle \n");
		break;
	case P1_WALK_BACK_IDLE:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&walkBackAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_IDLE;
		}
		//printf("walk_back_idle \n");
		break;
	case P1_IDLE_PUNCH:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &punchAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&punchAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_PUNCH_IDLE;
		}
		//printf("idle_punch\n");
		break;
	case P1_PUNCH_IDLE:
		if (animator.m_CurrentTime > 0.6 * (punchAnimationP1.GetDuration() * 1.0f)) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&punchAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.8f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
				charState = P1_IDLE;
			}
			//printf("punch_idle \n");
		}
		else {
			// punching
			//printf("punching \n");
		}
		break;
	case P1_IDLE_KICK:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &kickAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&kickAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_KICK_IDLE;
		}
		//printf("idle_kick\n");
		break;
	case P1_KICK_IDLE:
		if (animator.m_CurrentTime > 0.7f * kickAnimationP1.GetDuration()) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&kickAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.7f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
				charState = P1_IDLE;
			}
			//printf("kick_idle \n");
		}
		else {
			// punching
			//printf("kicking \n");
		}
		break;
	case P1_IDLE_BLOCK:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &blockAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&blockAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_BLOCK_IDLE;
		}
		//printf("idle_block\n");
		break;
	case P1_BLOCK_IDLE:
		if (animator.m_CurrentTime > 0.7f * (blockAnimationP1.GetDuration() * 0.5f)) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&blockAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.9f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
				charState = P1_IDLE;
			}
			//printf("block_idle \n");
		}
		else {
			// punching
			//printf("blocking \n");
		}
		break;
	case P1_IDLE_HIT:
		blendAmount += blendRate * 2;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP1, &hitAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&hitAnimationP1, NULL, startTime, 0.0f, blendAmount);
			charState = P1_HIT_IDLE;
		}
		//printf("idle_hit\n");
		break;
	case P1_HIT_IDLE:
		if (animator.m_CurrentTime > 0.7f * hitAnimationP1.GetDuration()) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&hitAnimationP1, &idleAnimationP1, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.7f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP1, NULL, startTime, 0.0f, blendAmount);
				charState = P1_IDLE;
			}
			//printf("hit_idle \n");
		}
		else {
			// punching
			//printf("blocking \n");
		}
		break;
	}
}


void UpdateStateP2(GLFWwindow* window, Animator& animator, AnimStateP2& charState, float& blendAmount)
{


	switch (charState) {
	case P2_IDLE:
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			blendAmount = 0.0f;
			player2Position.z -= moveSpeed * deltaTime;
			animator.PlayAnimation(&idleAnimationP2, &walkFrontAnimationP2, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P2_IDLE_WALK_FRONT;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			blendAmount = 0.0f;
			player2Position.z += moveSpeed * deltaTime;
			animator.PlayAnimation(&idleAnimationP2, &walkBackAnimationP2, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P2_IDLE_WALK_BACK;
		}


		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP2, &blockAnimationP2, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P2_IDLE_BLOCK;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP2, &punchAnimationP2, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P2_IDLE_PUNCH;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
			blendAmount = 0.0f;
			animator.PlayAnimation(&idleAnimationP2, &kickAnimationP2, animator.m_CurrentTime, 0.0f, blendAmount);
			charState = P2_IDLE_KICK;
		}
		//printf("idle \n");
		break;
	case  P2_IDLE_WALK_FRONT:
		player2Position.z -= moveSpeed * deltaTime;
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP2, &walkFrontAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&walkFrontAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_WALK_FRONT;
		}
		//printf("idle_walk_front \n");
		break;
	case  P2_IDLE_WALK_BACK:
		player2Position.z += moveSpeed * deltaTime;
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP2, &walkBackAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.8f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&walkBackAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_WALK_BACK;
		}
		//printf("idle_walk_back \n");
		break;
	case  P2_WALK_FRONT:
		player2Position.z -= moveSpeed * deltaTime;
		animator.PlayAnimation(&walkFrontAnimationP2, NULL, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS) {
			charState = P2_WALK_FRONT_IDLE;

		}
		//printf("walking_front\n");
		break;
	case  P2_WALK_BACK:
		player2Position.z += moveSpeed * deltaTime;
		animator.PlayAnimation(&walkBackAnimationP2, NULL, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS) {
			charState = P2_WALK_BACK_IDLE;

		}
		//printf("walking_back\n");
		break;
	case  P2_WALK_FRONT_IDLE:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&walkFrontAnimationP2, &idleAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.8f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&idleAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_IDLE;
		}
		//printf("walk_front_idle \n");
		break;
	case  P2_WALK_BACK_IDLE:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&walkBackAnimationP2, &idleAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&idleAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_IDLE;
		}
		//printf("walk_back_idle \n");
		break;
	case  P2_IDLE_PUNCH:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP2, &punchAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.8f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&punchAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_PUNCH_IDLE;
		}
		//printf("idle_punch\n");
		break;
	case  P2_PUNCH_IDLE:
		if (animator.m_CurrentTime > 0.65 * punchAnimationP2.GetDuration()) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&punchAnimationP2, &idleAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.6f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP2, NULL, startTime, 0.0f, blendAmount);
				charState = P2_IDLE;
			}
			//printf("punch_idle \n");
		}
		else {
			// punching
			//printf("punching \n");
		}
		break;
	case  P2_IDLE_KICK:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP2, &kickAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&kickAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_KICK_IDLE;
		}
		//printf("idle_kick\n");
		break;
	case  P2_KICK_IDLE:
		if (animator.m_CurrentTime > 0.6f * kickAnimationP2.GetDuration()) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&kickAnimationP2, &idleAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.7f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP2, NULL, startTime, 0.0f, blendAmount);
				charState = P2_IDLE;
			}
			//printf("kick_idle \n");
		}
		else {
			// punching
			//printf("kicking \n");
		}
		break;
	case  P2_IDLE_BLOCK:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		animator.PlayAnimation(&idleAnimationP2, &blockAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = animator.m_CurrentTime2;
			animator.PlayAnimation(&blockAnimationP2, NULL, startTime, 0.0f, blendAmount);
			charState = P2_BLOCK_IDLE;
		}
		//printf("idle_block\n");
		break;
	case  P2_BLOCK_IDLE:
		if (animator.m_CurrentTime > 0.7f * (blockAnimationP2.GetDuration() * 0.5f)) {
			blendAmount += blendRate;
			blendAmount = fmod(blendAmount, 1.0f);
			animator.PlayAnimation(&blockAnimationP2, &idleAnimationP2, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
			if (blendAmount > 0.9f) {
				blendAmount = 0.0f;
				float startTime = animator.m_CurrentTime2;
				animator.PlayAnimation(&idleAnimationP2, NULL, startTime, 0.0f, blendAmount);
				charState = P2_IDLE;
			}
			//printf("block_idle \n");
		}
		else {
			// punching
			//printf("blocking \n");
		}
		break;

	}
}

>>>>>>> Stashed changes


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("anim_model.vs", "anim_model.fs");
	Player player1("Object/Vegas/Big Vegas.dae", glm::vec3(0.0f, -0.4f, 0.0f), glm::vec3(1.0f, 2.0f, 1.0f), player1Anims);


	// load models
	// -----------
	// idle 3.3, walk 2.06, run 0.83, punch 1.03, kick 1.6
	
<<<<<<< Updated upstream
	//enum AnimState charState = IDLE;
	//float blendAmount = 0.0f;
	//float blendRate = 0.055f;

=======
	player1.loadModel("Object/Vegas/Big Vegas.dae");
	introAnimationP1.loadAnimation("Object/Vegas/Step Hip Hop Dance.dae",&player1);
	idleAnimationP1.loadAnimation("Object/Vegas/Idle.dae",&player1);
	walkFrontAnimationP1.loadAnimation("Object/Vegas/WalkForward.dae", &player1,1.8f);
	walkBackAnimationP1.loadAnimation("Object/Vegas/WalkBack.dae", &player1,1.5f);
	punchAnimationP1.loadAnimation("Object/Vegas/Punch Combo.dae", &player1,1.5f);
	kickAnimationP1.loadAnimation("Object/Vegas/Kicking.dae", &player1,1.5f);
	blockAnimationP1.loadAnimation("Object/Vegas/Bouncing Fight Idle.dae", &player1,1.2f);
	hitAnimationP1.loadAnimation("Object/Vegas/Head Hit.dae", &player1, 1.5f);

	player2.loadModel("Object/Wrestler/Ch43_nonPBR.dae");
	introAnimationP2.loadAnimation("Object/Wrestler/Catwalk Walk.dae", &player2);
	idleAnimationP2.loadAnimation("Object/Wrestler/Idle.dae", &player2);
	walkFrontAnimationP2.loadAnimation("Object/Wrestler/Walking.dae", &player2, 1.5f);
	walkBackAnimationP2.loadAnimation("Object/Wrestler/Standing Walk Back.dae", &player2, 1.7f);
	punchAnimationP2.loadAnimation("Object/Wrestler/Cross Punch.dae", &player2, 1.7f);
	punchAnimationP2.AddDamageKeyframe(0.5f,P2punchDamage);
	kickAnimationP2.loadAnimation("Object/Wrestler/Mma Kick.dae", &player2, 1.8f);
	blockAnimationP2.loadAnimation("Object/Wrestler/Center Block.dae", &player2, 1.5f);


	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);
	pbrShader.setInt("albedoMap", 3);
	pbrShader.setInt("normalMap", 4);
	pbrShader.setInt("metallicMap", 5);
	pbrShader.setInt("roughnessMap", 6);
	pbrShader.setInt("aoMap", 7);


	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	initTextRendering("Textures/Fonts/Roboto-Bold.ttf");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	initUIRendering();
	unsigned int healthBarTexture = loadTexture("Textures/UI/Test.png");


	// pbr: setup framebuffer
   // ----------------------
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// pbr: load the HDR environment map
	// ---------------------------------
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf("Textures/HDR/newport_loft.hdr", &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::cout << "HDR Loaded: Width = " << width << ", Height = " << height << ", Components = " << nrComponents << std::endl;

		stbi_image_free(data);

	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))

	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	equirectangularToCubemapShader.use();
	equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	equirectangularToCubemapShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	// --------------------------------------------------------------------------------
	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	// --------------------------------------------------------------------------------
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	// ----------------------------------------------------------------------------------------------------
	prefilterShader.use();
	prefilterShader.setInt("environmentMap", 0);
	prefilterShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pbr: generate a 2D LUT from the BRDF equations used.
	// ----------------------------------------------------
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	pbrShader.use();
	for (int i = 0; i < 4; ++i) {
		pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
		pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
	}


>>>>>>> Stashed changes
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	setupIntro();

	camera.Yaw = introP1camYaw;
	camera.Pitch = introP1camPitch;

	camera.updateCameraVectors();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
<<<<<<< Updated upstream
		processInput(window);
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			player1.Action("idle", false);  // No blending when switching to idle
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			player1.Action("walk", true);  // Blend into walking
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			player1.Action("punch", true);  // Blend into punching
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			player1.Action("kick", true);  // Blend into kicking
		}
		
		//switch (charState) {
		//case IDLE:
		//	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//		blendAmount = 0.0f;
		//		animator.PlayAnimation(&idleAnimation, &walkAnimation, animator.m_CurrentTime, 0.0f, blendAmount);
		//		charState = IDLE_WALK;
		//	}
		//	else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		//		blendAmount = 0.0f;
		//		animator.PlayAnimation(&idleAnimation, &punchAnimation, animator.m_CurrentTime, 0.0f, blendAmount);
		//		charState = IDLE_PUNCH;
		//	}
		//	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		//		blendAmount = 0.0f;
		//		animator.PlayAnimation(&idleAnimation, &kickAnimation, animator.m_CurrentTime, 0.0f, blendAmount);
		//		charState = IDLE_KICK;
		//	}
		//	printf("idle \n");
		//	break;
		//case IDLE_WALK:
		//	blendAmount += blendRate;
		//	blendAmount = fmod(blendAmount, 1.0f);
		//	animator.PlayAnimation(&idleAnimation, &walkAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//	if (blendAmount > 0.9f) {
		//		blendAmount = 0.0f;
		//		float startTime = animator.m_CurrentTime2;
		//		animator.PlayAnimation(&walkAnimation, NULL, startTime, 0.0f, blendAmount);
		//		charState = WALK;
		//	}
		//	printf("idle_walk \n");
		//	break;
		//case WALK:
		//	animator.PlayAnimation(&walkAnimation, NULL, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//	if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS) {
		//		charState = WALK_IDLE;
		//	}
		//	printf("walking\n");
		//	break;
		//case WALK_IDLE:
		//	blendAmount += blendRate;
		//	blendAmount = fmod(blendAmount, 1.0f);
		//	animator.PlayAnimation(&walkAnimation, &idleAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//	if (blendAmount > 0.9f) {
		//		blendAmount = 0.0f;
		//		float startTime = animator.m_CurrentTime2;
		//		animator.PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
		//		charState = IDLE;
		//	}
		//	printf("walk_idle \n");
		//	break;
		//case IDLE_PUNCH:
		//	blendAmount += blendRate;
		//	blendAmount = fmod(blendAmount, 1.0f);
		//	animator.PlayAnimation(&idleAnimation, &punchAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//	if (blendAmount > 0.9f) {
		//		blendAmount = 0.0f;
		//		float startTime = animator.m_CurrentTime2;
		//		animator.PlayAnimation(&punchAnimation, NULL, startTime, 0.0f, blendAmount);
		//		charState = PUNCH_IDLE;
		//	}
		//	printf("idle_punch\n");
		//	break;
		//case PUNCH_IDLE:
		//	if (animator.m_CurrentTime > 0.7 * punchAnimation.GetDuration()) {
		//		blendAmount += blendRate;
		//		blendAmount = fmod(blendAmount, 1.0f);
		//		animator.PlayAnimation(&punchAnimation, &idleAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//		if (blendAmount > 0.9f) {
		//			blendAmount = 0.0f;
		//			float startTime = animator.m_CurrentTime2;
		//			animator.PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
		//			charState = IDLE;
		//		}
		//		printf("punch_idle \n");
		//	}
		//	else {
		//		// punching
		//		printf("punching \n");
		//	}
		//	break;
		//case IDLE_KICK:
		//	blendAmount += blendRate;
		//	blendAmount = fmod(blendAmount, 1.0f);
		//	animator.PlayAnimation(&idleAnimation, &kickAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//	if (blendAmount > 0.9f) {
		//		blendAmount = 0.0f;
		//		float startTime = animator.m_CurrentTime2;
		//		animator.PlayAnimation(&kickAnimation, NULL, startTime, 0.0f, blendAmount);
		//		charState = KICK_IDLE;
		//	}
		//	printf("idle_kick\n");
		//	break;
		//case KICK_IDLE:
		//	if (animator.m_CurrentTime > 0.7f * kickAnimation.GetDuration()) {
		//		blendAmount += blendRate;
		//		blendAmount = fmod(blendAmount, 1.0f);
		//		animator.PlayAnimation(&kickAnimation, &idleAnimation, animator.m_CurrentTime, animator.m_CurrentTime2, blendAmount);
		//		if (blendAmount > 0.9f) {
		//			blendAmount = 0.0f;
		//			float startTime = animator.m_CurrentTime2;
		//			animator.PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
		//			charState = IDLE;
		//		}
		//		printf("kick_idle \n");
		//	}
		//	else {
		//		// punching
		//		printf("kicking \n");
		//	}
		//	break;
		//}



		
=======
		//processInput(window);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
>>>>>>> Stashed changes

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();


		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		auto transforms = player1.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		player1.draw(ourShader);
		player1.updateAnimation(deltaTime);

		//glm::mat4 model2 = glm::mat4(1.0f);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
		//ourShader.setMat4("model", model2);
		//guitarModel.Draw(ourShader);

<<<<<<< Updated upstream

=======
		


		switch (currentState) {
		case INTRO_P1:
		case INTRO_P2:
			updateIntro(window, deltaTime);
			break;
		case COUNTDOWN:
			startCountdown(deltaTime);
			break;
		case GAMEPLAY:
			// Gameplay logic
			updateCapsules();
			handleCollisions();
			UpdateStateP1(window, player1_animator, P1charState, blendAmountP1);
			UpdateStateP2(window, player2_animator, P2charState, blendAmountP2);
			timer.update();
			break;
		}

		player1_animator.UpdateAnimation(deltaTime);
		player2_animator.UpdateAnimation(deltaTime);
		
		if (gameStart) {

			UIShader.use();
			RenderHealthBars(UIShader, healthBarTexture);

			std::string timerText = timer.getFormattedTime();
			RenderText(textShader, timerText, (SCR_WIDTH / 2.0f) - 20.0f , static_cast<float>(SCR_HEIGHT) - 100.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		}
>>>>>>> Stashed changes

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

bool checkCollision(const BoxCollider& a, const BoxCollider& b) {
	// Check if boxes overlap on all axes
	bool xCollide = abs(a.center.x - b.center.x) * 2 < (a.size.x + b.size.x);
	bool yCollide = abs(a.center.y - b.center.y) * 2 < (a.size.y + b.size.y);
	bool zCollide = abs(a.center.z - b.center.z) * 2 < (a.size.z + b.size.z);
	return xCollide && yCollide && zCollide;
}

void updateGame() {

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
<<<<<<< Updated upstream
=======



	
// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);


}



void initTextRendering(const std::string& fontPath) {
	// Initialize FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	if (fontPath.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load fontPath" << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Done_FreeType(ft);
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	// Load first 128 characters of ASCII
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Set up text VAO/VBO
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color) {

	glUseProgram(shader.ID); // Use text shader
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Ensure proper blending for text
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

unsigned int loadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void initUIRendering() {
	float vertices[] = {
		// Positions   // Texture Coords
		0.0f, 1.0f,   0.0f, 1.0f,  // Top-left
		1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
		0.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left

		0.0f, 1.0f,   0.0f, 1.0f,  // Top-left
		1.0f, 1.0f,   1.0f, 1.0f,  // Top-right
		1.0f, 0.0f,   1.0f, 0.0f   // Bottom-right
	};

	glGenVertexArrays(1, &uiVAO);
	glGenBuffers(1, &uiVBO);
	glBindVertexArray(uiVAO);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderUIElement(Shader& shader, unsigned int texture, float x, float y, float width, float height) {
	shader.use();

	// Set up an orthographic projection for UI rendering
	glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
	shader.setMat4("projection", projection);

	// Transform for positioning and scaling the UI element
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));
	shader.setMat4("model", model);

	// Bind texture and render the quad
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(uiVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
>>>>>>> Stashed changes
