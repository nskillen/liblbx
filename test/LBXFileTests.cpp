#include <bandit/bandit.h>

#include <memory>

#include "../src/LBXFile.h"

using namespace OpenMOO2::LBX;

typedef std::unique_ptr<LBXFile> LBXFile_ptr;

using namespace bandit;

go_bandit([]() {
	describe("LBXFile", []() {
		LBXFile_ptr lbx_file;

		before_each([&]() {
			lbx_file = LBXFile_ptr(new LBXFile());
		});

		it("contains no data", [&]() {
			AssertThat(lbx_file->num_packed_files(), Equals(0));
			AssertThat(lbx_file->size(), Equals(8));
		});

		describe("loading an LBX file", [&]() {
			before_each([&]() {
				
			});
		});
	});
});

int main(int argc, char** argv) {
	return bandit::run(argc, argv);
}