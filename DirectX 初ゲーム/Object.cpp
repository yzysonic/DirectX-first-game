#include "Object.h"
#include "ObjectManager.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
Transform* newTransform(Object* object);


Transform* newTransform(Object* object)
{
	Transform *transform = New(Transform);

	transform->object		= object;
	transform->position		= Vector3(0, 0, 0);
	transform->rotation		= Vector3(0, 0, 0);
	transform->scale		= Vector3(1, 1, 1);

	return transform;
}



Object* newObject(void *owner, ObjectType type)
{
	Object *thiz = ObjectManager_GetObj();

	if (thiz != NULL)
	{
		thiz->owner = owner;
		thiz->transform		= newTransform(thiz);
		thiz->polygon		= NULL;
		thiz->rigidbody		= NULL;
		thiz->collider		= NULL;
		thiz->type			= type;
		thiz->isActive		= false;
		thiz->updateIndex	= -1;

		thiz->init			= GetObjectFunc(type)->init;
		thiz->update		= GetObjectFunc(type)->update;
		thiz->uninit		= GetObjectFunc(type)->uninit;
		thiz->onCollision	= GetObjectFunc(type)->onCollision;

		strcpy(thiz->name, "Object");

		Object_SetActive(thiz, true);

		if(owner != NULL)
			((ObjOwner*)owner)->base = thiz;

		if (thiz->init)
			thiz->init(thiz);
	}

	return thiz;
}

void deleteObject(Object* thiz)
{
	if (thiz == NULL)
		return;

	Object_SetActive(thiz, false);
	SafeDelete(thiz->transform);
	deleteRigidbody(thiz->rigidbody);
	deleteCollider(thiz->collider);
	deletePolygon(thiz->polygon);

	ObjOwner* owner = (ObjOwner*)thiz->owner;

	if (thiz->uninit != NULL)
		thiz->uninit(thiz);

	ObjectManager_ReleaseObj(owner->base);

}

void Object_SetActive(Object* thiz, bool value)
{
	if (thiz->update == NULL)
		return;

	if (thiz->isActive != value)
	{
		if (value)
		{
			if (ObjectManager_UpdateList_Add(thiz))
				thiz->isActive = true;
		}
		else
		{
			ObjectManager_UpdateList_Remove(thiz);
			thiz->isActive = false;
		}
	}
}