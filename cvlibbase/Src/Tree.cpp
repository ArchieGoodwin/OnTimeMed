/*!
 * \file	PumpABC.cpp
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include <assert.h>
#include "Tree.h"

namespace CVLib
{

void ListNode::Detach()
{
	if( Parent() == 0 )
		return;
 	
 	if( parent->first == this )
		parent->first = next;
 	if( parent->last == this )
		parent->last = prev;
	parent = 0;
	
	if( prev != 0 )
		prev->next = next;
	if( next != 0 )
		next->prev = prev;
	prev = next = 0;
}

void List::DetachAll()
{
	ListNode* node = first;
	while( node != 0 ) {
		ListNode* next = node->next;
		node->next = node->prev = 0;
		node->parent = 0;
		node = next;
	}
	first = last = 0;
}

void List::DeleteAll()
{
	while( !IsEmpty() )
		delete first;
	first = last = 0;
}

//-------------------------------------------------------------------------------------------

void List::Sort( int (*fcmp)( const ListNode*, const ListNode* ) )
{
	if( IsEmpty() )
		return;
	for( ListNode* listEnd = First(); listEnd->Next() != 0; ) {
		if( fcmp( listEnd, listEnd->Next() ) <= 0 )
			listEnd = listEnd->Next();
		else {
			ListNode* node = listEnd->Next();
			node->Detach();
			ListNode* insBefore=NULL;
			for( insBefore = First(); fcmp(insBefore, node) <= 0; 
			insBefore = insBefore->Next() )
				;
			insBefore->InsertPrev( node );
		}
	}
}

void List::AddListFirst( List& toAdd )
{
	if( toAdd.IsEmpty() )
		return;

	for( ListNode* ptr = toAdd.First(); ptr != 0; ptr = ptr->Next() )
		ptr->parent = this;
	
	if( IsEmpty() ) {
		last = toAdd.Last();
	} else {
		toAdd.Last()->next = First();
		First()->prev = toAdd.Last();
	}
	first = toAdd.First();
	toAdd.first = toAdd.last = 0;
}

void List::AddListLast( List& toAdd )
{
	if( toAdd.IsEmpty() )
		return;

	for( ListNode* ptr = toAdd.First(); ptr != 0; ptr = ptr->Next() )
		ptr->parent = this;
	
	if( IsEmpty() ) {
		first = toAdd.First();
	} else {
		toAdd.First()->prev = Last();
		Last()->next = toAdd.First();
	}
	last = toAdd.Last();
	toAdd.first = toAdd.last = 0;
}

// class CListSortInfo
// {
// public:
// 	CListSortInfo() : CompareFunc(0) {}
// 
// 	int (*CompareFunc)(const ListNode*, const ListNode*);
// };
// 
// static CThreadLocal<CListSortInfo> sortInfo;
// 
// static int listCmp( const const ListNode* *n1, const const ListNode* *n2 )
// {
// 	return sortInfo->CompareFunc( *n1, *n2 );
// }
// 
// void CList::QSort( int (*fcmp)(const ListNode*, const ListNode*) )
// {
// 	CListSortArray buffer;
// 
// 	for( ListNode* node = First(); node != 0; node = node->Next() )
// 		buffer.Add( node );
// 
// 	sortInfo->CompareFunc = fcmp;
// 	buffer.QSort( listCmp );
// 
// 	for( int i = 0; i < buffer.Size(); i++ ) {
// 		ListNode*(buffer[i])->Detach();
// 		AddLast( ListNode*(buffer[i]) );
// 	}
// }

int List::NumberOfChildren() const
{
	int count = 0;
	for( const ListNode* node = First(); node != 0; node = node->Next() )
		count++;
	return count;
}

}
