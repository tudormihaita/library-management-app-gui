#pragma once

class Tests {
public:
	void runAllTests();
private:
	void runDomainTests();
	void runValidationTests();
	void runMemoryRepoTests();
	void runFileRepoTests();
	void runLabRepoTests();
	void runServiceTests();
	void runWishlistTests();
	void runUndoTests();
};