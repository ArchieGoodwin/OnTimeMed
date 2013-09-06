/*!
 * \file	Tree.h
 * \ingroup base
 * \brief	Tree¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _Tree_H_
#define _Tree_H_

#include "PtrArray.h"
#include "SString.h"
#include "CoImage.h"

namespace CVLib
{
class List;
class ListNode;
class Tree;

#define	DECLARE_STD_NODE_METHODS(nodeClass, listClass)	\
public:	const nodeClass* Prev() const;\
const nodeClass* Next() const;\
const listClass* Parent() const;\
nodeClass* Prev();\
nodeClass* Next();\
listClass* Parent();

#define	IMPLEMENT_STD_NODE_METHODS(nodeClass, listClass)	\
inline const nodeClass* nodeClass::Prev() const { return static_cast<const nodeClass*>(ListNode::Prev()); }	\
inline const nodeClass* nodeClass::Next() const { return static_cast<const nodeClass*>(ListNode::Next()); }	\
inline const listClass* nodeClass::Parent() const { return static_cast<const listClass*>(ListNode::Parent()); } \
inline nodeClass* nodeClass::Prev() { return static_cast<nodeClass*>(ListNode::Prev()); }	\
inline nodeClass* nodeClass::Next() { return static_cast<nodeClass*>(ListNode::Next()); }	\
inline listClass* nodeClass::Parent() { return static_cast<listClass*>(ListNode::Parent()); }

class CVLIB_DECLSPEC ListNode {
public:
	ListNode() { parent = 0; prev = next = 0; }
	virtual ~ListNode() { Detach(); }

	const ListNode* Prev() const { return prev; }
	const ListNode* Next() const { return next; }
	const List* Parent() const { return parent; }
	ListNode* Prev() { return prev; }
	ListNode* Next() { return next; }
	List* Parent() { return parent; }

	void InsertPrev( ListNode* toIns );
	void InsertNext( ListNode* toIns );

	void Detach();

private:
	ListNode( const ListNode& ); // Withoun body
	ListNode& operator = ( ListNode& ); // Withoun body

	List* parent;
	ListNode* prev;
	ListNode* next;

	friend class List;
};

//------------------------------------------------------------------------
#define	DECLARE_STD_LIST_METHODS(listClass, nodeClass)	\
public:	const nodeClass* First() const;\
const nodeClass* Last() const;\
nodeClass* First();\
nodeClass* Last();

#define	IMPLEMENT_STD_LIST_METHODS(listClass, nodeClass)	\
inline const nodeClass* listClass::First() const { return static_cast<const nodeClass*>(List::First()); }	\
inline const nodeClass* listClass::Last() const { return static_cast<const nodeClass*>(List::Last()); }	\
inline nodeClass* listClass::First() { return static_cast<nodeClass*>(List::First()); }	\
inline nodeClass* listClass::Last() { return static_cast<nodeClass*>(List::Last()); }

class CVLIB_DECLSPEC List {
public:
	List() { first = last = 0; }
	virtual ~List() { DeleteAll(); }

	const ListNode* First() const { return first; }
	const ListNode* Last() const { return last; }
	ListNode* First() { return first; }
	ListNode* Last() { return last; }

	int NumberOfChildren() const;
	BOOL IsEmpty() const { return first == 0; }
	BOOL HasChild( const ListNode* child ) const { return this == child->Parent(); }

	void AddFirst( ListNode* child );
		// adds the element to the top of list
	void AddLast( ListNode* child );
		// adds the element to the end of list
	void AddListFirst( List& toAdd );
	void AddListLast( List& toAdd );
	void DeleteAll(); 
	void DetachAll(); 

	void Sort( int (*fcmp)( const ListNode*, const ListNode* ) );
	void QSort( int (*fcmp)(const ListNode*, const ListNode*) );

protected:
	ListNode* first;
	ListNode* last;

private:
	List( const List& ); // Withoun body
	const List& operator = ( const List& ); // Withoun body

	friend class ListNode;
};

//-------------------------------------------------------------------------
#define	DEFINE_STD_TREE_METHODS(nodeClass)	\
public:	const nodeClass* Prev() const { return static_cast<const nodeClass*>(Tree::Prev()); }	\
const nodeClass* Next() const { return static_cast<const nodeClass*>(Tree::Next()); }	\
const nodeClass* Parent() const { return static_cast<const nodeClass*>(Tree::Parent()); }	\
const nodeClass* First() const { return static_cast<const nodeClass*>(Tree::First()); }	\
const nodeClass* Last() const { return static_cast<const nodeClass*>(Tree::Last()); }	\
nodeClass* Prev() { return static_cast<nodeClass*>(Tree::Prev()); }	\
nodeClass* Next() { return static_cast<nodeClass*>(Tree::Next()); }	\
nodeClass* Parent() { return static_cast<nodeClass*>(Tree::Parent()); }	\
nodeClass* First() { return static_cast<nodeClass*>(Tree::First()); }	\
nodeClass* Last() { return static_cast<nodeClass*>(Tree::Last()); }

#define	DECLARE_STD_TREE_METHODS(nodeClass, parentClass, childClass)	\
public:	const nodeClass* Prev() const;\
const nodeClass* Next() const;\
const parentClass* Parent() const;\
const childClass* First() const;\
const childClass* Last() const;\
nodeClass* Prev();\
nodeClass* Next();\
parentClass* Parent();\
childClass* First();\
childClass* Last();

#define	IMPLEMENT_STD_TREE_METHODS(nodeClass, parentClass, childClass)	\
inline const nodeClass* nodeClass::Prev() const { return static_cast<const nodeClass*>(Tree::Prev()); }	\
inline const nodeClass* nodeClass::Next() const { return static_cast<const nodeClass*>(Tree::Next()); }	\
inline const parentClass* nodeClass::Parent() const { return static_cast<const parentClass*>(Tree::Parent()); }	\
inline const childClass* nodeClass::First() const { return static_cast<const childClass*>(Tree::First()); }	\
inline const childClass* nodeClass::Last() const { return static_cast<const childClass*>(Tree::Last()); }	\
inline nodeClass* nodeClass::Prev() { return static_cast<nodeClass*>(Tree::Prev()); }	\
inline nodeClass* nodeClass::Next() { return static_cast<nodeClass*>(Tree::Next()); }	\
inline parentClass* nodeClass::Parent() { return static_cast<parentClass*>(Tree::Parent()); }	\
inline childClass* nodeClass::First() { return static_cast<childClass*>(Tree::First()); }	\
inline childClass* nodeClass::Last() { return static_cast<childClass*>(Tree::Last()); }

class CVLIB_DECLSPEC Tree : private ListNode, private List {
public:
	const Tree* Prev() const { return (const Tree*) ListNode::Prev(); }
	const Tree* Next() const { return (const Tree*) ListNode::Next(); }
	const Tree* Parent() const { return (const Tree*) ListNode::Parent(); }
	const Tree* First() const { return (const Tree*) List::First(); }
	const Tree* Last() const { return (const Tree*) List::Last(); }
	Tree* Prev() { return (Tree*) ListNode::Prev(); }
	Tree* Next() { return (Tree*) ListNode::Next(); }
	Tree* Parent() { return (Tree*) ListNode::Parent(); }
	Tree* First() { return (Tree*) List::First(); }
	Tree* Last() { return (Tree*) List::Last(); }

	BOOL HasChild( const Tree* child ) const { return List::HasChild( child ); }
	
	void InsertPrev( Tree* toIns ) { ListNode::InsertPrev( toIns ); }
	void InsertNext( Tree* toIns ) { ListNode::InsertNext( toIns ); }

	void AddFirst( Tree* child ) { List::AddFirst( child ); }
		// add childs to the top of childs' list
	void AddLast( Tree* child ) { List::AddLast( child ); }
		// add childs to the end of childs' list

	void AddTreeFirst( Tree& tree ) { List::AddListFirst( tree ); }
	void AddTreeLast( Tree& tree ) { List::AddListLast( tree ); }

 	void Sort( int (*fcmp)( const Tree*, const Tree* ) )
 		{ List::Sort( (int (*)(const ListNode*, const ListNode*)) fcmp ); }
// 	void QSort( int (*fcmp)( const Tree*, const Tree* ) )
// 		{ List::QSort( (int (*)(const ListNode*, const ListNode*)) fcmp ); }

		
	using List::IsEmpty;
	using List::DeleteAll;
	using List::DetachAll;
	using List::NumberOfChildren;
	
	using ListNode::Detach;
};

//----------------------------------------------------------------------
inline void ListNode::InsertPrev( ListNode* toIns )
{
	assert( toIns->Parent() == 0 && Parent() != 0 );

	if( prev != 0 ) {
		toIns->prev = prev;
    	prev->next = toIns;
	} else
		parent->first = toIns;

	toIns->next = this;
	prev = toIns;
	toIns->parent = parent;
}

inline void ListNode::InsertNext( ListNode* toIns )
{
	assert( toIns->Parent() == 0 && Parent() != 0 );

	if( next != 0 ) {
		toIns->next = next;
		next->prev = toIns;
	} else
		parent->last = toIns;

	toIns->prev = this;
	next = toIns;
	toIns->parent = parent;
}

inline void List::AddFirst( ListNode* child )
{
	assert( child->Parent() == 0 );
 	if( first != 0 ) {
		first->InsertPrev( child );
	} else {
		first = last = child;
		child->parent = this;
	}
}

inline void List::AddLast( ListNode* child )
{
	assert( child->Parent() == 0 );
 	if( last != 0 ) {
		last->InsertNext( child );
	} else {
		first = last = child;
 		child->parent = this;
	}
}

}

#endif //_Tree_H_