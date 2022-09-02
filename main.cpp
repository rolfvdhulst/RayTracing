#include <iostream>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <execution>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "src/Random.h"
#include "src/Vec3.h"
#include "src/Ray.h"
#include "src/Sphere.h"
#include "src/Scene.h"
#include "src/Camera.h"
#include "src/BoundingVolumeHierarchy.h"

std::pair<Scene,Camera>  exampleScene(RandomDevice& device, Real aspectRatio) {
  Scene scene;

  Material groundMat = scene.addMaterial(DiffuseMaterial(Vec3r(0.8,0.8,0.0)));
  Material ballMat  = scene.addMaterial(DiffuseMaterial(Vec3r(0.1,0.2,0.5)));
  Material glassMat = scene.addMaterial(DielectricMaterial(1.3));
  Material metalMat = scene.addMaterial(MetalMaterial(Vec3r(0.8,0.6,0.2),0.0));

  scene.addSphere(SphereData(Vec3r(0.0, -100.5, -1.0), 100,groundMat));
  scene.addSphere(SphereData(Vec3r(0.0, 0.0, -1.0), 0.5,ballMat));
  scene.addSphere(SphereData(Vec3r(-1.0, 0.0, -1.0), 0.5, glassMat));
  scene.addSphere(SphereData(Vec3r(-1.0, 0.0, -1.0), -0.45, glassMat));

  scene.addSphere(SphereData(Vec3r(1.0, 0.0, -1.0), 0.5, metalMat,Vec3r(50.0,0.0,0.0)));
  scene.setBackgroundColor(Vec3r(0.7,0.8,1.0));

  Vec3r lookfrom(13, 2, 3);
  Vec3r lookat(0, 0, 0);
  Real distToFocus = 10.0;
  Real aperture = 0.1;
  Real shutterTime = Real(1 / 250.0);

  Camera camera(lookfrom, lookat, Vec3r(0, 1, 0), 20, aspectRatio, aperture, distToFocus,shutterTime);
  scene.initialize(shutterTime,device);
  scene.setBackgroundColor(Vec3r(0.7,0.8,1.0));

  return std::make_pair(scene,camera);
}

std::pair<Scene,Camera> randomScene(RandomDevice& device, Real aspectRatio) {
  Scene scene;

  Material material = scene.addMaterial(DiffuseMaterial(Vec3r(0.5,0.5,0.5)));
  scene.addSphere(SphereData(Vec3d(0, -1000, 0), 1000, material));

  for (int a = -11; a < 11; a++) {
	for (int b = -11; b < 11; b++) {
	  auto chooseMat = device.randomReal();
	  Vec3d center(a + 0.9 * device.randomReal(), 0.2, b + 0.9 * device.randomReal());
	  Vec3r velocity(0.0,0.0,0.0);
	  if ((center - Vec3d(4, 0.2, 0)).norm() > 0.9) {
		Material sphereMaterial;

		if (chooseMat < 0.7) {
		  // diffuse
		  Vec3d color1 = device.randomVec();
		  Vec3d color2 = device.randomVec();
		  sphereMaterial = scene.addMaterial(DiffuseMaterial(color1*color2));
		  velocity.y() = device.randomReal(0.0,25.0);
		} else if (chooseMat < 0.9) {
		  // metal
		  sphereMaterial = scene.addMaterial(
			  MetalMaterial(0.5*device.randomVec()+0.5,device.randomReal(0.0,0.5))
			  );

		} else {
		  sphereMaterial = scene.addMaterial(DielectricMaterial(device.randomReal(1.1, 1.7)));
		}
		scene.addSphere(SphereData(center, 0.2, sphereMaterial, velocity));
	  }
	}
  }

  material = scene.addMaterial(DielectricMaterial(1.5));
  scene.addSphere(SphereData(Vec3d(0, 1, 0), 1, material));

  material = scene.addMaterial(DiffuseMaterial(Vec3r(0.4, 0.2, 0.1)));
  scene.addSphere(SphereData(Vec3d(-4, 1, 0), 1, material));

  material = scene.addMaterial(MetalMaterial(Vec3r(0.7,0.6,0.5),0.0));
  scene.addSphere(SphereData(Vec3d(4, 1, 0), 1, material));


  Vec3r lookfrom(13, 2, 3);
  Vec3r lookat(0, 0, 0);
  Real distToFocus = 10.0;
  Real aperture = 0.1;
  Real shutterTime = Real(1 / 250.0);

  Camera camera(lookfrom, lookat, Vec3r(0, 1, 0), 20, aspectRatio, aperture, distToFocus,shutterTime);
  scene.initialize(shutterTime,device);
  scene.setBackgroundColor(Vec3r(0.7,0.8,1.0));

  return std::make_pair(scene,camera);
}

std::pair<Scene,Camera> randomSceneDark(RandomDevice& device, Real aspectRatio) {
  Scene scene;

  Material material = scene.addMaterial(DiffuseMaterial(Vec3r(0.5,0.5,0.5)));
  scene.addSphere(SphereData(Vec3d(0, -1000, 0), 1000, material));

  for (int a = -11; a < 11; a++) {
	for (int b = -11; b < 11; b++) {
	  auto chooseMat = device.randomReal();
	  Vec3d center(a + 0.9 * device.randomReal(), 0.2, b + 0.9 * device.randomReal());
	  Vec3r velocity(0.0,0.0,0.0);
	  if ((center - Vec3d(4, 0.2, 0)).norm() > 0.9) {
		Material sphereMaterial;

		if (chooseMat < 0.7) {
		  // diffuse
		  Vec3d color1 = device.randomVec();
		  Vec3d color2 = device.randomVec();
		  sphereMaterial = scene.addMaterial(DiffuseMaterial(color1*color2));
		  velocity.y() = device.randomReal(0.0,25.0);
		} else if (chooseMat < 0.9) {
		  // metal
		  sphereMaterial = scene.addMaterial(
			  MetalMaterial(0.5*device.randomVec()+0.5,device.randomReal(0.0,0.5))
		  );

		} else {
		  sphereMaterial = scene.addMaterial(DielectricMaterial(device.randomReal(1.1, 1.7)));
		}
		scene.addSphere(SphereData(center, 0.2, sphereMaterial, velocity));
	  }
	}
  }

  material = scene.addMaterial(DielectricMaterial(1.5));
  scene.addSphere(SphereData(Vec3d(0, 1, 0), 1, material));

  material = scene.addMaterial(DiffuseMaterial(Vec3r(0.4, 0.2, 0.1),Vec3r(10.0,10.0,10.0)));
  scene.addSphere(SphereData(Vec3d(-4, 1, 0), 1, material));

  material = scene.addMaterial(MetalMaterial(Vec3r(0.7,0.6,0.5),0.0));
  scene.addSphere(SphereData(Vec3d(4, 1, 0), 1, material));


  Vec3r lookfrom(13, 2, 3);
  Vec3r lookat(0, 0, 0);
  Real distToFocus = 10.0;
  Real aperture = 0.1;
  Real shutterTime = Real(1 / 250.0);

  Camera camera(lookfrom, lookat, Vec3r(0, 1, 0), 20, aspectRatio, aperture, distToFocus,shutterTime);
  scene.initialize(shutterTime,device);
  scene.setBackgroundColor(Vec3r(0.0,0.0,0.0));

  return std::make_pair(scene,camera);
}

std::pair<Scene,Camera> simpleLight(RandomDevice& device, Real aspectRatio){
  Scene scene;

  Vec3r lookfrom(26,3,6);
  Vec3r lookat(0, 2, 0);
  Real aperture = 0.1;
  Real shutterTime = Real(1 / 250.0);

  Camera camera(lookfrom, lookat, Vec3r(0, 1, 0), 20, aspectRatio, aperture, (lookfrom-lookat).norm(),shutterTime);

  Material sphereMat = scene.addMaterial(DiffuseMaterial(Vec3r(0.5,0.8,0.2)));

  scene.addSphere(SphereData(Vec3r(0.0,-1000.0,0.0),1000,sphereMat));
  scene.addSphere(SphereData(Vec3r(0.0,2.0,0.0),2,sphereMat));

  Material light = scene.addMaterial(DiffuseMaterial(Vec3r(0.0,0.0,0.0),Vec3r(4.0,4.0,4.0)));

  scene.addRectangle(AARectangleData(light,RectangleType::xy,3,5,1,3,-2));

  scene.initialize(shutterTime,device);
  scene.setBackgroundColor(Vec3r(0.0,0.0,0.0));
  return std::make_pair(scene,camera);
}

std::pair<Scene,Camera> cornellBox(RandomDevice& device, Real aspectRatio){
  Scene scene;

  Vec3r lookfrom(278,278,-800);
  Vec3r lookat(278,278, 0);
  Real aperture = 0.1;
  Real shutterTime = Real(1 / 250.0);
  Real fov = 40.0;

  Camera camera(lookfrom, lookat, Vec3r(0, 1, 0), fov, aspectRatio, aperture, (lookfrom-lookat).norm(),shutterTime);


  Material red = scene.addMaterial(DiffuseMaterial(Vec3r(.65,.05,.05)));
  Material white = scene.addMaterial(DiffuseMaterial(Vec3r(.73,.73,.73)));
  Material green = scene.addMaterial(DiffuseMaterial(Vec3r(.12,.45,.15)));

  Material light= scene.addMaterial(DiffuseMaterial(Vec3r(.0,.0,.0),Vec3r(15,15,15)));

  scene.addRectangle(AARectangleData(green,RectangleType::yz,0,555,0,555,555));
  scene.addRectangle(AARectangleData(red,RectangleType::yz,0,555,0,555,0));
  scene.addRectangle(AARectangleData(light,RectangleType::zx,213,343,227,332,554));
  scene.addRectangle(AARectangleData(white,RectangleType::zx,0,555,0,555,0));
  scene.addRectangle(AARectangleData(white,RectangleType::zx,0,555,0,555,555));
  scene.addRectangle(AARectangleData(white,RectangleType::xy,0,555,0,555,555));

  scene.initialize(shutterTime,device);
  scene.setBackgroundColor(Vec3r(0.0,0.0,0.0));
  return std::make_pair(scene,camera);
}

Vec3r rayColor(const Ray &ray, const Scene &scene, int depth, RandomDevice& device) {
  if (depth <= 0) {
	return Vec3r(0, 0, 0);
  }
  std::optional<HitRecord> hit = scene.hit(ray, 0.001, std::numeric_limits<Real>::infinity());
  if (hit.has_value()) {
	Ray scattered;
	Vec3r attenuation;
	Vec3r emitted = scene.material(hit->material).emitted(hit.value()) ;
	bool scatter = scene.material(hit->material).scatter(ray,hit.value(),scattered,attenuation,device);
	if(!scatter){
	  return emitted;
	}
	return emitted + attenuation * rayColor(scattered,scene,depth-1,device);
  }

  return scene.backgroundColor();
}

std::ostream &writeColor(std::ostream &ostream, const Vec3r &color, int samplesPerPixel) {
  Vec3r actualColor = color;
  Real scale = (1.0 / samplesPerPixel);
  actualColor.x() = sqrt(scale * actualColor.x());
  actualColor.y() = sqrt(scale * actualColor.y());
  actualColor.z() = sqrt(scale * actualColor.z());

  ostream << static_cast<int>(255.999 * actualColor.x()) << ' ' <<
		  static_cast<int>(255.999 * actualColor.y()) << ' ' <<
		  static_cast<int>(255.999 * actualColor.z()) << '\n';
  return ostream;
}

struct BlockJob{
  int rowStart;
  int colStart;
  int numRows;
  int numCols;

  int samplesPerPixel;
  int maxDepth;
  RandomDevice device;
  std::vector<Vec3r> colors;
};
void runBlockJob(BlockJob& job,const Camera& camera, const Scene& scene, int imageWidth, int imageHeight){
  for (int j = job.rowStart; j < job.rowStart + job.numRows; ++j) {
	//std::cerr<<"starting line: "<<j<<'\r';
	for (int i = job.colStart; i < job.colStart + job.numCols; ++i) {
	  Vec3r pixelColor(0, 0, 0);
	  for (int k = 0; k < job.samplesPerPixel; ++k) {
		auto u = (i + job.device.randomReal()) / (imageWidth - 1);
		auto v = (j + job.device.randomReal()) / (imageHeight - 1);
		Ray ray = camera.getRay(u, v,job.device);
		pixelColor += rayColor(ray, scene, job.maxDepth,job.device);
	  }
	  int jobJ = j-job.rowStart;
	  int jobI = i-job.colStart;
	  job.colors[size_t(jobJ*job.numCols+jobI)] = pixelColor;
	}
  }
}
void runWorkerThread(const Camera& camera, const Scene& scene, int imageWidth, int imageHeight,
				 std::mutex& mutex, std::condition_variable& cv,
				 std::atomic<int>& completedThreads,
				 std::atomic<size_t>& nextJobIndex,
				 std::vector<BlockJob>& jobs,
				 std::vector<BlockJob>& imageBlocks){
  BlockJob job{
	.device = RandomDevice(1)
  };
  {
	std::lock_guard<std::mutex> lock(mutex);
	if(nextJobIndex == jobs.size()){
	  completedThreads++;
	  cv.notify_one();
	  return;
	}
	job = jobs[nextJobIndex];
	nextJobIndex++;
	std::cerr<<"starting with job: "<< nextJobIndex<<"/"<<jobs.size()<<"\r"<<std::flush;
  }
  while(true){
	runBlockJob(job,camera,scene,imageWidth,imageHeight);
	{
	  std::lock_guard<std::mutex> lock(mutex);
	  imageBlocks.push_back(job);
	  if(nextJobIndex == jobs.size()){
		completedThreads++;
		cv.notify_one();
		return;
	  }else{
		job = jobs[nextJobIndex];
		nextJobIndex++;
		std::cerr<<"starting with job: "<< nextJobIndex<<"/"<<jobs.size()<<"\r"<<std::flush;
	  }
	}
  }
}

int main() {

  //Image
//  constexpr auto aspectRatio = 16.0 / 9.0;
//  constexpr int imageWidth = 1920;
//  constexpr int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  constexpr int samplesPerPixel = 50;
  constexpr int maxDepth = 50;


  //Scene

  RandomDevice64 rng(42);

  constexpr auto aspectRatio = 1.0;
  constexpr int imageWidth = 600;
  constexpr int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  auto pair = cornellBox(rng,aspectRatio);

  //Render

  std::atomic<int> completedThreads = 0;
  std::mutex mutex;
  std::condition_variable cvResults;
  std::vector<BlockJob> imageBlocks;
  std::vector<std::thread> threads;
  constexpr size_t nThreads = 8;

  //Define the chunks/jobs to be handled
  std::atomic<size_t> nextJobIndex = 0;
  std::vector<BlockJob> jobs;
  //Job for each row: option
  {
	for (int row = 0; row < imageHeight; ++row) {
	  jobs.push_back(BlockJob{
		  .rowStart = row,
		  .colStart = 0,
		  .numRows = 1,
		  .numCols = imageWidth,
		  .samplesPerPixel = samplesPerPixel,
		  .maxDepth = maxDepth,
		  .device = RandomDevice((uint64_t)(row + 1)),
		  .colors = std::vector<Vec3r>(imageWidth)
	  });
	}
  }

  //One job for each thread
//  {
//	int intNThreads = nThreads;
//	int rowsPerThread = imageHeight / intNThreads;
//	int leftOver = imageHeight % intNThreads;
//	for (int i = 0; i < intNThreads; ++i) {
//	  int numRows = i == (intNThreads-1) ? (rowsPerThread+leftOver) : rowsPerThread;
//	  int numCols = imageWidth;
//	  jobs.push_back(BlockJob{
//		  .rowStart = i*rowsPerThread,
//		  .colStart = 0,
//		  .numRows = numRows,
//		  .numCols = numCols,
//		  .samplesPerPixel = samplesPerPixel,
//		  .maxDepth = maxDepth,
//		  .device = RandomDevice((uint64_t)(i + 1)),
//		  .colors = std::vector<Vec3r>(size_t(numRows*numCols))
//	  });
//	}
//  }

  auto startTime = std::chrono::high_resolution_clock::now();

  {
	for (size_t i = 0; i < nThreads; ++i) {
	  std::thread thread([ pair, &mutex, &cvResults, &completedThreads, &imageBlocks,&nextJobIndex,&jobs]() {
		runWorkerThread(pair.second, pair.first, imageWidth, imageHeight, mutex, cvResults, completedThreads,
					nextJobIndex,jobs, imageBlocks);
	  });
	  threads.push_back(std::move(thread));
	}
  }
  auto afterThreadLaunchTime = std::chrono::high_resolution_clock::now();
  //Wait for the image
  {
	std::unique_lock<std::mutex> lock(mutex); //Why is this lock needed?
	cvResults.wait(lock,[&completedThreads](){return completedThreads == nThreads;});
  }
  for(auto& thread : threads){
	thread.join();
  }
  auto beforeCopyTime = std::chrono::high_resolution_clock::now();

  std::vector<Vec3r> colorBuffer(imageWidth * imageHeight);
  for(const auto& job : imageBlocks){
	for (int jobJ = 0; jobJ < job.numRows ; ++jobJ) {
	  for (int jobI = 0; jobI < job.numCols; ++jobI) {
		int totJ = job.rowStart + jobJ;
		int totI = job.colStart + jobI;
		colorBuffer[size_t(totJ*imageWidth+totI)] = job.colors[size_t(jobJ*job.numCols+jobI)];
	  }
	}
  }

  std::cerr<<"\n";
  auto endTime = std::chrono::high_resolution_clock::now();

  //write to file
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  for (int j = imageHeight-1 ; j >= 0 ; --j) {
	for (int i = 0; i < imageWidth; ++i) {
	  auto index = size_t(j*imageWidth + i);
	  writeColor(std::cout, colorBuffer[index], samplesPerPixel);
	}
  }
  std::cerr<<"Computation took "<<(static_cast<double>((beforeCopyTime-startTime).count())/1e9) <<
  " seconds, Copying took "<<(static_cast<double>((endTime-beforeCopyTime).count())/1e9)  <<"\n";
  std::cerr<<"Launching threads took: "<<(static_cast<double>((afterThreadLaunchTime-startTime).count())/1e9)<<" seconds\n";
  return 0;
}
