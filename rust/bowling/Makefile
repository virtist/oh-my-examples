all: test

.PHONY: test lcov clean

test:
	CARGO_INCREMENTAL=0 RUSTFLAGS='-Cinstrument-coverage' LLVM_PROFILE_FILE='cargo-test-%p-%m.profraw' cargo test --lib

lcov:
	grcov . --binary-path ./target/debug/deps/ \
		-s . \
		-t lcov \
		--branch --ignore-not-existing \
		--ignore '../*' --ignore "/*" \
		-o target/debug/lcov.info
	genhtml -o ./target/debug/coverage/ \
		--show-details \
		--highlight \
		--ignore-errors source \
		--legend ./target/debug/lcov.info

clean:
	cargo clean

	