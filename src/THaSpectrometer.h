#ifndef ROOT_THaSpectrometer
#define ROOT_THaSpectrometer

//////////////////////////////////////////////////////////////////////////
//
// THaSpectrometer
//
//////////////////////////////////////////////////////////////////////////

#include "THaApparatus.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TRotation.h"

class THaParticleInfo;
class THaPidDetector;
class THaTrack;

class THaSpectrometer : public THaApparatus {
  
public:
  virtual ~THaSpectrometer();
  
  virtual Int_t            AddDetector( THaDetector* det );
  virtual Int_t            AddPidParticle( const char* shortname, 
					   const char* name,
					   Double_t mass, Int_t charge = 0 );
  virtual Int_t            CalcPID();
          void             Clear( Option_t* opt="" );
  virtual void             DefinePidParticles();
  virtual Int_t            DefineVariables( EMode mode = kDefine );
  virtual Int_t            FindVertices( TClonesArray& tracks ) = 0;
          THaTrack*        GetGoldenTrack()   const { return fGoldenTrack; }
          Int_t            GetNpidParticles() const;
          Int_t            GetNpidDetectors() const;
  const   THaParticleInfo* GetPidParticleInfo( Int_t i ) const;
  const   THaPidDetector*  GetPidDetector( Int_t i ) const;
          Int_t            GetNTracks()  const { return fTracks->GetLast()+1; }
          TClonesArray*    GetTracks()   const { return fTracks; }
          TClonesArray*    GetTrackPID() const { return fTrackPID; }
  //          TClonesArray*    GetVertices() const { return fVertices; }
          Bool_t           IsPID() const       { return fPID; }
  virtual Int_t            Reconstruct();
          void             SetGoldenTrack( THaTrack* t ) { fGoldenTrack = t; }
          void             SetPID( Bool_t b = kTRUE )    { fPID = b; }
  virtual Int_t            TrackCalc() = 0;

  // The following is specific to small-acceptance pointing spectrometers
  // using spectrometer-specific coordinates such as TRANSPORT
  const   TRotation&       GetToLabRot() const { return fToLabRot; }
  const   TRotation&       GetToTraRot() const { return fToTraRot; }
  const   TVector3&        GetPointingOffset() const { return fPointingOffset; }
          Double_t         GetThetaGeo() const { return fThetaGeo; }
          Double_t         GetPhiGeo()   const { return fPhiGeo; }
          Double_t         GetThetaSph() const { return fThetaSph; }
          Double_t         GetPhiSph()   const { return fPhiSph; }
          Double_t         GetPcentral() const { return fPcentral; }
          Double_t         GetCollDist() const { return fCollDist; }

  virtual void             TrackToLab( THaTrack& track, TVector3& pvect ) const;
  virtual void             TransportToLab( Double_t p, Double_t th, 
					   Double_t ph, TVector3& pvect ) const;
  virtual void             LabToTransport( TVector3& vertex, TVector3& pvect,
					   TVector3& tvertex, 
					   Double_t* ray ) const;

protected:

  static const Int_t kInitTrackMultiplicity = 3;

  TClonesArray*   fTracks;                //Tracks 
  TClonesArray*   fTrackPID;              //PID info for the tracks
  //  TClonesArray*   fVertices;              //Track vertices
  TList*          fTrackingDetectors;     //Tracking detectors
  TList*          fNonTrackingDetectors;  //Non-tracking detectors
  TObjArray*      fPidDetectors;          //PID detectors
  TObjArray*      fPidParticles;          //Particles for which we want PID
  THaTrack*       fGoldenTrack;           //Golden track within fTracks
  Bool_t          fPID;                   //PID enabled

  // The following is specific to small-acceptance pointing spectrometers
  TRotation       fToLabRot;              //Rotation matrix from TRANSPORT to lab
  TRotation       fToTraRot;              //Rotation matrix from lab to TRANSPORT
  TVector3        fPointingOffset;        //Optical point in lab coordinate system
  Double_t        fThetaGeo;              //In-plane geographic central angle (rad)
  Double_t        fPhiGeo;                //Out-of-plane geographic central angle (rad)
  Double_t        fThetaSph, fPhiSph;     //Central angles in spherical coords. (rad)
  Double_t        fSinThGeo, fCosThGeo;   //Sine and cosine of central angles
  Double_t        fSinPhGeo, fCosPhGeo;   // in geographical coordinates
  Double_t        fSinThSph, fCosThSph;   //Sine and cosine of central angles in 
  Double_t        fSinPhSph, fCosPhSph;   // spherical coordinates
  Double_t        fPcentral;              //Central momentum (GeV)
  Double_t        fCollDist;              //Distance from collimator to target center (m)

  // only derived classes can construct me
  THaSpectrometer() {}
  THaSpectrometer( const char* name, const char* description );

private:
  Bool_t          fListInit;      //Detector lists initialized

  void            ListInit();     //Initializes lists of specialized detectors

  ClassDef(THaSpectrometer,0)     //A generic spectrometer
};


//---------------- inlines ----------------------------------------------------
inline Int_t THaSpectrometer::GetNpidParticles() const
{
  return fPidParticles->GetLast()+1;
}

//_____________________________________________________________________________
inline Int_t THaSpectrometer::GetNpidDetectors() const
{
  return fPidDetectors->GetLast()+1;
}

//_____________________________________________________________________________
inline const THaParticleInfo* THaSpectrometer::GetPidParticleInfo( Int_t i ) 
  const
{
  return (const THaParticleInfo*) fPidParticles->At(i);
}

//_____________________________________________________________________________
inline const THaPidDetector* THaSpectrometer::GetPidDetector( Int_t i ) const
{
  return (const THaPidDetector*) fPidDetectors->At(i);
}

#endif

