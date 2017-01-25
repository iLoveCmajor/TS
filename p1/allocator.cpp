#include "allocator.h"


Pointer::Pointer()
    : ptr(new void *(nullptr)),
    size(new size_t(0))
{}


Pointer::Pointer(void **p, size_t s)
    : ptr(p),
    size(new size_t(s))
{}

bool Pointer::operator==(const Pointer &p) const
{
    if (ptr == p.ptr && size == p.size)
        return true;
    else
        return false;
}

Allocator::Allocator(void *base, size_t s)
    : base_ptr(base),
    size(s),
    in_use(std::vector<bool>(s, 0)),
    ptrs(std::vector<Pointer *>())
	{}

int Allocator::getIdxOfPtr(const Pointer &p)
{
	for( int idx = 0; idx < ptrs.size(); idx++)
		if(*(ptrs[idx]) == p)
			return idx;

	return -1;
}

Pointer Allocator::alloc(size_t N)
{
	int start_ind = 0;
	int bytes = 0;
	bool mem_enough = false;
	for(int i = 0; i < size; ++i)
	{
		if(!in_use[i])
			bytes++;
		else
		{
			start_ind = i + 1;
			bytes = 0;
		}
		if(bytes == N)
		{
			mem_enough = true;
			break;
		}
	}
	if(!mem_enough)
		throw AllocError(AllocErrorType::NoMemory, "Not enoung memory\n");

	for(int k = 0; k < N; ++k)
		in_use[start_ind + k] = true;
	void **new_ptr = new void *((char *)base_ptr + start_ind);
	ptrs.push_back(new Pointer(new_ptr, N));

	return *ptrs[ptrs.size() - 1];
}

void Allocator::realloc(Pointer &p, size_t N)
{
	if(p.getSize() == 0)
		{
			p = alloc(N);
			return;
		}

	int offset = (char *)p.get() - (char *)base_ptr;
	if(N < p.getSize())
	{
		for(int i = N; i < p.getSize(); ++i)
			in_use[offset + i] = false;
		p.setSize(N);
		return;
	}

	bool extendable = true;
	for(int i = p.getSize(); i < N; ++i)
		if(in_use[offset + i])
		{
			extendable = false;
			break;
		}

	if(extendable)
	{
		for(int i = p.getSize(); i < N; ++i)
			in_use[offset + i] = true;
		p.setSize(N);
	}
	else
	{
		Pointer new_ptr = alloc(N);
		memcpy(new_ptr.get(), p.get(), p.getSize());
		Allocator::free(p);
		p = new_ptr;
	}
}

void Allocator::defrag()
{
    sort(ptrs.begin(), ptrs.end(), [](Pointer *p1, Pointer *p2) -> bool
    {
        return p1->get() < p1->get();
    });

    char *dst = (char *)base_ptr;
    int whole_length = 0;
    for (int i = 0; i < ptrs.size(); ++i)
    {
        int curr_size = ptrs[i]->getSize();
        memmove(dst, ptrs[i]->get(), curr_size);
        ptrs[i]->setPtr(dst);
        whole_length += curr_size;
        dst += curr_size;
    }
    for (int i = 0; i < whole_length; ++i)
        in_use[i] = true;
    for (int i = whole_length; i < size; ++i)
        in_use[i] = false;
}

void Allocator::free(Pointer &p)
{
	int offset = (char *)p.get() - (char *)base_ptr;
	int idx_of_ptr;
	if((idx_of_ptr == getIdxOfPtr(p)) == -1)
		throw AllocError(AllocErrorType::InvalidFree, "Invalid free\n");

	for(int i = 0; i < p.getSize(); ++i)
		in_use[offset + i] = false;
	delete ptrs[idx_of_ptr];
	ptrs.erase(ptrs.begin() + idx_of_ptr);
	p = Pointer( new void *(nullptr), 0);
}

std::string Allocator::dump()
{
    return "";
}


Allocator::~Allocator()
{
    for (int i = 0; i < ptrs.size(); ++i)
        delete ptrs[i];
}
