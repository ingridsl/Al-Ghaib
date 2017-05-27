#include "InventoryKey.hpp"

InventoryKey::InventoryKey(/*std::string file*/){
	//Acho melhor n�o passar como string pq a� s� est� atrelado a pr�pria classe, n�o depende de outra ter essa info
	sp = Sprite("img/minion.png");
}

InventoryKey::~InventoryKey(){
}

void InventoryKey::Render(int x, int y){
	sp.Render(x,y,0);
}

void InventoryKey::Action(){ }

bool InventoryKey::IsObject(std::string type){
	return (type == "KeyObject");
}

int InventoryKey::GetWidth(){ return sp.GetWidth();}

int InventoryKey::GetHeight(){ return sp.GetHeight(); }

void InventoryKey::SetScaleX(float scale) { sp.SetScaleX(scale);}

void InventoryKey::SetScaleY(float scale) { sp.SetScaleY(scale);}

