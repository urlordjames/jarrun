{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	buildInputs = with pkgs; [
		cmake
		ninja
		jdk
		curl
	];
}
