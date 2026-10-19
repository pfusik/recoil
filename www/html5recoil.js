// html5recoil.js - HTML 5 viewer
//
// Copyright (C) 2013-2023 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

import { RECOIL } from "./recoil.js";

let recoilFiles;

function recoil2canvas(mainFilename)
{
	const recoil = new RECOIL();
	recoil.readFile = (filename, content, contentLength) => {
		if (!recoilFiles.hasOwnProperty(filename))
			return -1;
		const source = recoilFiles[filename];
		if (contentLength > source.length)
			contentLength = source.length;
		for (let i = 0; i < contentLength; i++)
			content[i] = source[i];
		return contentLength;
	};

	const content = recoilFiles[mainFilename];
	if (!recoil.decode(mainFilename, content, content.length)) {
		alert("Error!");
		return;
	}

	const canvas = document.getElementById("canvas");
	const width = recoil.getWidth();
	const height = recoil.getHeight();
	const pixels = recoil.getPixels();
	canvas.width = width;
	canvas.height = height;
	const context = canvas.getContext("2d");
	const imageData = context.createImageData(width, height);
	for (let i = 0; i < width * height; i++) {
		const rgb = pixels[i];
		const j = i << 2;
		imageData.data[j] = rgb >> 16;
		imageData.data[j + 1] = rgb >> 8 & 0xff;
		imageData.data[j + 2] = rgb & 0xff;
		imageData.data[j + 3] = 0xff;
	}
	context.putImageData(imageData, 0, 0);

	const status = document.getElementById("status");
	status.innerHTML = recoil.getPlatform() + ", " + recoil.getOriginalWidth() + "x" + recoil.getOriginalHeight();

	if (canvas.webkitRequestFullScreen || canvas.mozRequestFullScreen)
		document.getElementById("fullScreenButton").style.display = "";

	const pngLink = document.getElementById("pngLink");
	pngLink.href = canvas.toDataURL("image/png");
	pngLink.download = mainFilename + ".png";
	pngLink.style.display = "";
}

function populateFiles(contents, mainFilenames)
{
	const select = document.getElementById("fileSelect");
	switch (mainFilenames.length) {
	case 0:
		alert("No supported file selected");
		return;
	case 1:
		select.style.display = "none";
		break;
	default:
		select.innerHTML = "";
		for (const name of mainFilenames)
			select.add(new Option(name, name));
		select.style.display = "";
		break;
	}
	recoilFiles = contents;
	recoil2canvas(mainFilenames[0]);
}

function openFiles()
{
	const files = this.files;
	const mainFilenames = [];
	const contents = {};
	let count = files.length;
	for (const file of files) {
		if (RECOIL.isOurFile(file.name))
			mainFilenames.push(file.name);
		const reader = new FileReader();
		reader.onload = e => {
			contents[file.name] = new Uint8Array(e.target.result);
			if (--count == 0)
				populateFiles(contents, mainFilenames);
		};
		reader.readAsArrayBuffer(file);
	}
}

document.getElementById("fileInput").addEventListener("change", openFiles);

document.getElementById("fileSelect").addEventListener("change", function() {
		recoil2canvas(this.value);
	});

document.getElementById("fullScreenButton").addEventListener("click", function() {
		document.getElementById("canvas").requestFullscreen();
	});
