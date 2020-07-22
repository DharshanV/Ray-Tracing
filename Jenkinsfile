pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        echo 'Build started'
        sh '''mkdir build
cd build
cmake ..
make
cd ..'''
      }
    }

    stage('Test') {
      parallel {
        stage('Test') {
          steps {
            echo 'Test run and Unit test'
          }
        }

        stage('Test run') {
          steps {
            echo 'Test run 500x500 2-split count'
            sh './build/render 500 500 2 renderedImage.ppm'
          }
        }

        stage('Unit Test') {
          steps {
            echo 'Unit Testing'
          }
        }

      }
    }

    stage('Clean Up') {
      steps {
        echo 'Cleaning up'
        sh '''echo "testing fail clean up"
dhusa'''
      }
    }

  }
}