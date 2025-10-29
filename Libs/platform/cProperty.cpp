#include "cProperty.h"

bool cBasePropety::valid(uint32 prop) const
{
	switch (prop)
	{
	case eProperty_Str:
	case eProperty_Agi:
	case eProperty_Vit:
	case eProperty_Int:
	case eProperty_Dex:
	case eProperty_Luk:
		return true;
	}
	return false;
}