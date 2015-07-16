#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Gearless/Gearless.hpp>

struct ABaseType
{
    int unused;
};

struct ADerivedType : public ABaseType
{
    int unused;
};

TEST_CASE("TypeId Generator should work with all possible arguments for the same types", "[TypeIdGen]")
{
    Gearless::TypeId baseTypeId = Gearless::GetTypeId<ABaseType>();
    Gearless::TypeId derivedTypeId = Gearless::GetTypeId<ADerivedType>();

    SECTION ("Check type id's for simple types")
    {
        REQUIRE(baseTypeId == Gearless::GetTypeId<ABaseType>());
    }

    SECTION ("Check type id's for derived types")
    {
        REQUIRE(derivedTypeId == Gearless::GetTypeId<ADerivedType>());
    }

    SECTION ("Check that type id's in base classes differ from derived types")
    {
        REQUIRE(baseTypeId != derivedTypeId);
    }

    SECTION ("Check that type id's are identified correctly using decltype")
    {
        ABaseType base;
        ADerivedType derived;
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<decltype(base)>());
        REQUIRE(Gearless::GetTypeId<ADerivedType>() == Gearless::GetTypeId<decltype(derived)>());
    }

    SECTION ("Check that type id's are the same when passing template parameters various const and reference identifiers")
    {
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<const ABaseType>());
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<ABaseType&>());
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<const ABaseType&>());
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<ABaseType&&>());
        REQUIRE(Gearless::GetTypeId<ABaseType>() == Gearless::GetTypeId<const ABaseType&&>());
    }
}
