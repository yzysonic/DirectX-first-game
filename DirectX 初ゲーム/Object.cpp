#include"Object.h"
#include"ObjectManager.h"
#include"Polygon.h"

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



Object* newObject(ObjectType type, void *owner)
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

		thiz->init			= ObjectManager_GetTypeFunc(type)->init;
		thiz->update		= ObjectManager_GetTypeFunc(type)->update;
		thiz->uninit		= ObjectManager_GetTypeFunc(type)->uninit;

		Object_SetActive(thiz, true);

		if (thiz->init)
			thiz->init(thiz);
	}

	return thiz;
}

void deleteObject(Object* thiz)
{
	if (thiz->uninit)
		thiz->uninit(thiz);

	Object_SetActive(thiz, false);
	SafeDelete(thiz->transform);
	SafeDelete(thiz->rigidbody);
	SafeDelete(thiz->collider);
	deletePolygon(thiz->polygon);

	thiz->owner = NULL;

	ObjectManager_ReleaseObj(thiz);
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