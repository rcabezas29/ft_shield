# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!

Vagrant.configure("2") do |config|
    config.vm.box = "symbols/ubuntu-minimal"
    config.vm.hostname = "ft-shield"

    config.ssh.forward_agent = true
    config.ssh.forward_x11 = true
    
    config.vm.provider "virtualbox" do |vb|
        vb.memory = 2048
    end

    $script = %Q{
        apt-get update
        sudo apt-get install git make -y
    }
    config.vm.provision :shell, :inline => $script
    config.vm.synced_folder ".", "/home/vagrant", disabled: false
end
