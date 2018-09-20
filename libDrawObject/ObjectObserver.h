#pragma once

struct ATTR_VALUE;
class CDrawObject;
/*!
 * \brief
 * 绘图对象改变事件响应接口类
 * 
 * 接口用于Undo、Redo时的相应操作
 * 
 * \remarks
 * 对一个图形对象的操作可能有：添加、删除子对象，修改对象属性，改变对象顺序等操作。
 * 因此， Undo、Redo调用时，当前Histry日志根据自身对象实现，调用此接口不同方法进行恢复或重做操作。
 * 
 * \see
 * CDrawObject|CHistry
 */
class IObjectObserver
{
public:


	/*!
	 * \brief
	 * 对象添加事件响应
	 * 
	 * \param container
	 * 添加对象的容器对象。
	 * 
	 * \param childToAdd
	 * 被添加的子对象。
	 * 
	 * \param afterObject
	 * 子对象插入后，其后续对象的指针。
	 * 
	 * \see
	 * CHistoryAddObject
	 */
	virtual void notifyChildAdded(CDrawObject * container, CDrawObject *childToAdd, CDrawObject *afterObject);

	/*!
	 * \brief
	 * 对象删除事件响应
	 * 
	 * \param changedObject
	 * 删除对象的容器对象。
	 * 
	 * \param childToDelete
	 * 被删除的子对象。
	 * 
	 * \param afterObject
	 * 被删除对象删除前的后续对象。
	 * 
	 * Write detailed description for notifyChildDeleted here.
	 * 
	 * \remarks
	 * Write remarks for notifyChildDeleted here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	virtual void notifyChildDeleted(CDrawObject *changedObject, CDrawObject *childToDelete, CDrawObject * afterObject);
    
    /*!
	 * \brief
	 * 对象属性改变响应.
	 * 
	 * \param changedObject
	 * 属性改变的对象
	 * 
	 * \param attributeIndex
	 * 属性索引
	 * 
	 * \param newVal
	 * 属性改变后的新值
	 * 
	 * \param oldVal
	 * 属性改变前的值
	 * 
	 * Write detailed description for notifyAttribtesChanged here.
	 * 
	 * \remarks
	 * Write remarks for notifyAttribtesChanged here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	virtual void notifyAttributesChanged(CDrawObject *changedObject, UINT attributeIndex, ATTR_VALUE &newVal,  ATTR_VALUE & oldVal );
	/*!
	 * \brief
	 * 对象顺序改变响应
	 * 
	 * \param container
	 * 顺序改变对象所在容器对象
	 * 
	 * \param changedObject
	 * 顺序被改变的对象
	 * 
	 * \param newAfterObject
	 * 新位置的后续对象指针
	 * 
	 * \param oldAfterObject
	 * 旧位置的后续对象指针
	 * 
	 * Write detailed description for notifyOrderChanged here.
	 * 
	 * \remarks
	 * Write remarks for notifyOrderChanged here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	virtual void notifyOrderChanged(CDrawObject* container, CDrawObject * changedObject, CDrawObject * newAfterObject, CDrawObject *oldAfterObject);
    virtual void notifyRevert(CDrawObject *changed, int &action );

	virtual ~IObjectObserver(){}
};
