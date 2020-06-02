
// DuilibSkinEditorDoc.h : CDuilibSkinEditorDoc ��Ľӿ�
//


#pragma once


class CDuilibSkinEditorDoc : public CDocument
{
protected: // �������л�����
	CDuilibSkinEditorDoc();
	DECLARE_DYNCREATE(CDuilibSkinEditorDoc)

// ����
public:

// ����
public:

// ��д
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

// ʵ��
public:
	virtual ~CDuilibSkinEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
