#pragma once

namespace Fiea
{
	namespace GameEngine
	{
		// Trasfrom struct
		struct Transform
		{
			glm::vec4 position = glm::vec4(0.0f);
			glm::vec4 rotation = glm::vec4(0.0f);
			glm::vec4 scale = glm::vec4(1.0f);

			/*
			* Function to translate the object
			* @param offset: The offset to translate the object
			*/
			void translate(const glm::vec3& offset)
			{
				position += glm::vec4(offset, 0.0f);
			}
			
			/*
			* Function to rotate the object
			* @param angle: The angle to rotate the object
			*/
			void rotate(float angle, const glm::vec3& axis)
			{
				glm::quat quaternionRotation = glm::angleAxis(angle, axis);
				glm::quat currentRotation = glm::quat(rotation);
				glm::quat newRotation = quaternionRotation * currentRotation;
				rotation = glm::vec4(glm::eulerAngles(newRotation), 0.0f);
			}

			/*
			* Function to return the right direction
			*/
			glm::vec4 right() const
			{
				glm::mat4 rotationMatrix = getRotationMatrix();
				return glm::vec4(rotationMatrix[0]);
			}

			/*
			* Function to return the up direction
			*/
			glm::vec4 up() const
			{
				glm::mat4 rotationMatrix = getRotationMatrix();
				return glm::vec4(rotationMatrix[1]);
			}

			/*
			* Function to return the forward direction
			*/
			glm::vec4 forward() const
			{
				glm::mat4 rotationMatrix = getRotationMatrix();
				return glm::vec4(rotationMatrix[2]);
			}

			/*
			* Function to return the euler angles
			*/
			glm::vec3 eulerAngles() const
			{
				glm::quat quaternionRotation = glm::quat(rotation);
				return glm::eulerAngles(quaternionRotation);
			}

			/*
			* Function to return the quaternion
			*/
			glm::quat quaternion() const
			{
				return glm::quat(rotation);
			}

			/*
			* Function to return the rotation matrix
			*/
			glm::mat4 getRotationMatrix() const
			{
				glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
				matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
				matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
				return matrix;
			}
		};
	}
}