#include <gtest/gtest.h>

extern "C" {
#include <stdint.h>
#include <errno.h>

#include <dev/ebpf/ebpf_map.h>
}

namespace {
class HashTableMapUpdateTest : public ::testing::Test {
      protected:
	struct ebpf_map map;

	virtual void
	SetUp()
	{
		int error;

		struct ebpf_map_attr attr;
		attr.type = EBPF_MAP_TYPE_HASHTABLE;
		attr.key_size = sizeof(uint32_t);
		attr.value_size = sizeof(uint32_t);
		attr.max_entries = 100;
		attr.flags = 0;

		error = ebpf_map_init(&map, &attr);
		ASSERT_TRUE(!error);
	}

	virtual void
	TearDown()
	{
		ebpf_map_deinit(&map, NULL);
	}
};

TEST_F(HashTableMapUpdateTest, CorrectUpdate)
{
	int error;
	uint32_t key = 50, value = 100;

	error = ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_ANY);

	EXPECT_EQ(0, error);
}

TEST_F(HashTableMapUpdateTest, CorrectUpdateMoreThanMaxEntries)
{
	int error;
	uint32_t i;

	for (i = 0; i < 100; i++) {
		error = ebpf_map_update_elem_from_user(&map, &i, &i, EBPF_ANY);
		ASSERT_TRUE(!error);
	}

	error = ebpf_map_update_elem_from_user(&map, &i, &i, EBPF_ANY);
	EXPECT_EQ(EBUSY, error);
}

TEST_F(HashTableMapUpdateTest, UpdateExistingElementWithNOEXISTFlag)
{
	int error;
	uint32_t key = 50, value = 100;

	error = ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_ANY);
	ASSERT_TRUE(!error);

	error =
	    ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_NOEXIST);

	EXPECT_EQ(EEXIST, error);
}

TEST_F(HashTableMapUpdateTest, UpdateNonExistingElementWithNOEXISTFlag)
{
	int error;
	uint32_t key = 50, value = 100;

	error =
	    ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_NOEXIST);

	EXPECT_EQ(0, error);
}

TEST_F(HashTableMapUpdateTest, UpdateNonExistingElementWithEXISTFlag)
{
	int error;
	uint32_t key = 50, value = 100;

	error = ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_EXIST);

	EXPECT_EQ(ENOENT, error);
}

TEST_F(HashTableMapUpdateTest, UpdateExistingElementWithEXISTFlag)
{
	int error;
	uint32_t key = 50, value = 100;

	error = ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_ANY);

	value++;
	error = ebpf_map_update_elem_from_user(&map, &key, &value, EBPF_EXIST);

	EXPECT_EQ(0, error);
}
} // namespace
