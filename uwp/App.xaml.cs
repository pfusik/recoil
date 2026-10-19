// App.xaml.cs - Universal Windows application
//
// Copyright (C) 2014-2022 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

using System.Linq;
using Windows.ApplicationModel.Activation;
using Windows.Storage;
using Windows.UI.Xaml;

namespace RECOIL
{
	sealed partial class App : Application
	{
		public App()
		{
			InitializeComponent();
		}

		static void Launch()
		{
			if (Window.Current.Content == null)
				Window.Current.Content = new MainPage();
			Window.Current.Activate();
		}

		protected override void OnLaunched(LaunchActivatedEventArgs e) => Launch();

		protected override async void OnFileActivated(FileActivatedEventArgs args)
		{
			base.OnFileActivated(args);
			Launch();
			if (Window.Current.Content is MainPage page)
				await page.ShowFiles(args.Files.OfType<StorageFile>().ToList());
		}
	}
}
