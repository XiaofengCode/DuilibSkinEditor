
// DuilibSkinEditorDoc.h : CDuilibSkinEditorDoc 类的接口
//


#pragma once


class CDuilibSkinEditorDoc : public CDocument
{
protected: // 仅从序列化创建
	CDuilibSkinEditorDoc();
	DECLARE_DYNCREATE(CDuilibSkinEditorDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void OnDocumentEvent(DocumentEvent deEvent);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual void OnSettingsChanged();

// 实现
public:
	virtual ~CDuilibSkinEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
