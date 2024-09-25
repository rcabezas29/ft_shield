Vagrant.configure("2") do |config|
	config.vm.box = "debian/bookworm64"

	config.vm.provider "virtualbox" do |vb|
		vb.memory = 4096
		vb.cpus = 4
	end

	config.vm.synced_folder "./", "/home/ft_shield", create: true

	config.vm.define "shield" do |shield_config|
		shield_config.vm.hostname = "shield"
		
		shield_config.vm.provision "shell", inline: <<-SHELL
			apt update && apt upgrade -y
			apt install build-essential git binutils libssl-dev -y
		SHELL
	end
end
