#include "Corredor.hpp"
#include "EstadosJugador.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"
#include "CajaFalsa.hpp"
#include "Aceite.hpp"
#include "Pista.hpp"

class CorredorRed : public Corredor
{
private:

public:
	CorredorRed(btVector3 pos, Corredor::tipo_jugador tipo);
	~CorredorRed();
	//virtual void update() override;
	virtual void updateHijos() override;
	virtual void movimiento() override;
	virtual void actualizarItem() override;
	void acelerarRed();
	void frenarRed();
	void girarDRed();
	void girarIRed();
};