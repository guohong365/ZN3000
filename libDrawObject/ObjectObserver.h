#pragma once

struct ATTR_VALUE;
class CDrawObject;
/*!
 * \brief
 * ��ͼ����ı��¼���Ӧ�ӿ���
 * 
 * �ӿ�����Undo��Redoʱ����Ӧ����
 * 
 * \remarks
 * ��һ��ͼ�ζ���Ĳ��������У���ӡ�ɾ���Ӷ����޸Ķ������ԣ��ı����˳��Ȳ�����
 * ��ˣ� Undo��Redo����ʱ����ǰHistry��־�����������ʵ�֣����ô˽ӿڲ�ͬ�������лָ�������������
 * 
 * \see
 * CDrawObject|CHistry
 */
class IObjectObserver
{
public:


	/*!
	 * \brief
	 * ��������¼���Ӧ
	 * 
	 * \param container
	 * ��Ӷ������������
	 * 
	 * \param childToAdd
	 * ����ӵ��Ӷ���
	 * 
	 * \param afterObject
	 * �Ӷ�����������������ָ�롣
	 * 
	 * \see
	 * CHistoryAddObject
	 */
	virtual void notifyChildAdded(CDrawObject * container, CDrawObject *childToAdd, CDrawObject *afterObject);

	/*!
	 * \brief
	 * ����ɾ���¼���Ӧ
	 * 
	 * \param changedObject
	 * ɾ���������������
	 * 
	 * \param childToDelete
	 * ��ɾ�����Ӷ���
	 * 
	 * \param afterObject
	 * ��ɾ������ɾ��ǰ�ĺ�������
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
	 * �������Ըı���Ӧ.
	 * 
	 * \param changedObject
	 * ���Ըı�Ķ���
	 * 
	 * \param attributeIndex
	 * ��������
	 * 
	 * \param newVal
	 * ���Ըı�����ֵ
	 * 
	 * \param oldVal
	 * ���Ըı�ǰ��ֵ
	 * 
	 * Write detailed description for notifyAttribtesChanged here.
	 * 
	 * \remarks
	 * Write remarks for notifyAttribtesChanged here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	virtual void notifyAttribtesChanged(CDrawObject *changedObject, UINT attributeIndex, ATTR_VALUE &newVal,  ATTR_VALUE & oldVal );
	/*!
	 * \brief
	 * ����˳��ı���Ӧ
	 * 
	 * \param container
	 * ˳��ı����������������
	 * 
	 * \param changedObject
	 * ˳�򱻸ı�Ķ���
	 * 
	 * \param newAfterObject
	 * ��λ�õĺ�������ָ��
	 * 
	 * \param oldAfterObject
	 * ��λ�õĺ�������ָ��
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
