#pragma once
#ifndef __VEHICLE_MOVEMENT_H__
#define __VEHICLE_MOVEMENT_H__

#include "SeparityExports\SeparityApi.h"
#include "EntityComponent\Component.h"

namespace Spyutils {
	class Vector3;
}
namespace Separity {	
	class RigidBody;
	class Transform;
}
namespace CrazyU {
	/// <summary>
	/// Componente VehicleMovement
	/// </summary>
	class VehicleMovement : public Separity::Component {
		public:
		__CMPTYPE_DECL__(Separity::_SCRIPT)
		__CMPID_DECL__(Separity::_GAME_COMPONENT)

		VehicleMovement(Separity::Transform* cameraTr);
		~VehicleMovement();

		void initComponent() override;
		/// <summary>
		/// A�ade giro al volante
		/// </summary>
		/// <param name="dir">Entre -1 y 1, siendo -1 izquierda y 1
		/// derecha</param>
		void girar(int dir);
		/// <summary>
		/// Acelera hacia delante o hacia atr�s
		/// <param name="dir">Entre -1 y 1, siendo -1 atr�s y 1 adelante</param>
		/// </summary>
		void acelerar(int dir);
		/// <summary>
		/// Gestiona el freno del veh�culo
		/// </summary>
		void frenar();
		/// <summary>
		/// M�todo update heredado de Component
		/// </summary>
		void update(const uint32_t& deltaTime) override;

		private:
		Separity::RigidBody* rb_;
		Separity::Transform* cameraTr_;
		float cameraOffset_ = 0;
		float cameraRot_ = 0;
	};
}  // namespace Separity
#endif __VEHICLE_MOVEMENT_H__