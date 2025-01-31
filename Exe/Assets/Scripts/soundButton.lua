--Definicion de la clase en Lua
soundButton = {}

soundButton.__index = soundButton

--Constructor de la clase en Lua
function soundButton:new()
	local obj={}
	setmetatable(obj, soundButton)
	return obj
end
local volumeOn=true;
--Metodo Awake, llamado al comienzo de la ejecucion
function soundButton:awake()

end

--Metodo Start, llamado tras Awake
function soundButton:start()

end

--Metodo Update, llamado en cada frame
function soundButton:update()

end

--Metodo OnCollisionEnter, llamado al comenzar una colision
function soundButton:onCollisionEnter()

end

--Metodo OnCollisionStay, llamado mientras se mantenga una colision
function soundButton:onCollisionStay()

end

--Metodo OnCollisionExit, llamado al salir de una colision
function soundButton:onCollisionExit()

end

--Metodo OnButtonClick, llamado al pulsar el boton de esta entidad
function soundButton:onButtonClick()
	print("Estoy encima del boton");
	audio = this.entity:getAudio();
	if audio then
		AudioManager:play(audio:getAudioName(), 10000, 10000)
	end
end

--Metodo OnButtonReleased, llamado al soltar el boton de esta entidad
function soundButton:onButtonReleased()
	if volumeOn then
		butt=soundButton.entity:getButton();
		butt:changeButtonTexture("noVolume","noVolume","noVolume");	
		AudioManager:turnOffVolume();
		volumeOn=false;
    else
		butt=soundButton.entity:getButton();
		butt:changeButtonTexture("volumeIni","volumeHover","volumeClick");	
		AudioManager:turnOnVolume();
		volumeOn=true;
    end
	
end

--Metodo OnButtonHover, llamado al posar el raton sobre el boton de esta entidad
function soundButton:onButtonHover()

end

--Metodo OnButtonUnhover, llamado al dejar de posar el raton sobre el boton de esta entidad
function soundButton:onButtonUnhover()

end

--Variable global de la clase (para Luabridge)
soundButtonLua = soundButton:new()
