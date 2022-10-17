#pragma once
#include <Windows.h>
#include <vector>


template<typename T>
class hook_t
{
private:
	bool enabled = false;
	const std::uintptr_t addy = 0;
	const std::size_t length = 0;
	std::size_t tweaked_length = 0;

	void* original_bytes = nullptr;
public:
	hook_t(auto addy, std::size_t length) : addy{ *reinterpret_cast<std::uintptr_t*>(&addy) }, length{ length }
	{
		this->tweaked_length = this->length + 5;

		DWORD old_protection;
		this->original_bytes = VirtualAlloc(nullptr, this->tweaked_length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		// Make sure we have read permissions to the address we're reading
		VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, PAGE_EXECUTE_READ, &old_protection);
		memcpy(this->original_bytes, reinterpret_cast<LPVOID>(this->addy), this->length);
		VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, old_protection, &old_protection);

		*reinterpret_cast<byte*>(reinterpret_cast<std::uintptr_t>(this->original_bytes) + this->length) = 0xE9;
		*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this->original_bytes) + this->length + 1) = this->addy - reinterpret_cast<std::uintptr_t>(this->original_bytes) - 5;

		// Tip for noobs: NEVER LEAVE YOUR PAGES WITH EXECUTE_READWRITE YOU'RE ASKING FOR AN ACE VULN IF YOU DO
		VirtualProtect(reinterpret_cast<LPVOID>(this->original_bytes), this->tweaked_length, PAGE_EXECUTE_READ, &old_protection);
	}

	hook_t(const hook_t&) = delete;

	~hook_t()
	{
		std::printf("deconstructing hook!\n");
		this->disable_hook();

		if (this->original_bytes)
		{
			VirtualFree(this->original_bytes, NULL, MEM_RELEASE);
		}
	}

	// Important! Make sure your scope doesn't die, otherwise the hook will deconstruct and poof your hook goes!
	void enable_hook(auto redirect_to, T& output)
	{
		if (!this->enabled)
		{
			this->enabled = true;
			
			DWORD old_protection;
			VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, PAGE_EXECUTE_READWRITE, &old_protection);
			*reinterpret_cast<byte*>(this->addy) = 0xE9;
			*reinterpret_cast<std::uintptr_t*>(this->addy + 1) = *reinterpret_cast<std::uintptr_t*>(&redirect_to) - this->addy - 5;
			VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, old_protection, &old_protection);

			output = *reinterpret_cast<T*>(&this->original_bytes);
		}
	}

	void disable_hook()
	{
		if (this->enabled)
		{
			this->enabled = false;

			DWORD old_protection;
			VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, PAGE_EXECUTE_READWRITE, &old_protection);
			memcpy(reinterpret_cast<LPVOID>(this->addy), this->original_bytes, this->length);
			VirtualProtect(reinterpret_cast<LPVOID>(this->addy), this->length, old_protection, &old_protection);
		}
	}
};