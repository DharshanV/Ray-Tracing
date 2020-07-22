pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        echo 'Build started'
        sh '''mkdir build
cd build
cmake ..
make'''
      }
    }

  }
}