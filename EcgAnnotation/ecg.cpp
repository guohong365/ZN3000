/*

ecg.cpp - ECG annotation console app based lib.cpp ECG library.
Copyright (C) 2007 YURIY V. CHESNOKOV

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


You may contact the author by e-mail (chesnokov.yuriy@gmail.com or chesnokov_yuriy@mail.ru)
or postal mail (Unilever Centre for Molecular Sciences Informatics,
University Chemical Laboratory, Cambridge University, Lensfield Road, Cambridge, CB2 1EW, UK)

*/


#include "stdafx.h"
#include "lib\lib.h"

wchar_t params[_MAX_PATH] = L"params";

void tic();
void toc();
void help();
int parse_params(class EcgAnnotation &ann);
void change_extension(wchar_t* path, const wchar_t* ext);

int _tmain(int argc, _TCHAR* argv[])
{
        wchar_t annName[_MAX_PATH];
        wchar_t hrvName[_MAX_PATH];
        
        if (argc < 2) {
                help();
        } else {
                int leadNumber = 0;
                if (argc >= 2 + 1) {
                        leadNumber = _wtoi(argv[2]) - 1;
                        if (leadNumber < 0) leadNumber = 0;
                }                

                class Signal signal;
                if (signal.ReadFile(argv[1])) {

                        int size = signal.GetLength();
                        double sr = signal.GetSR();
                        int h, m, s, ms;
                        int msec = int(((double)size / sr) * 1000.0);
                        signal.mSecToTime(msec, h, m, s, ms);

                        wprintf(L"  leads: %d\n", signal.GetLeadsNum());
                        wprintf(L"     sr: %.2lf Hz\n", sr);
                        wprintf(L"   bits: %d\n", signal.GetBits());
                        wprintf(L"    UmV: %d\n", signal.GetUmV());
                        wprintf(L" length: %02d:%02d:%02d.%03d\n\n", h, m, s, ms);
                        
                        double* data = signal.GetData(leadNumber);

                        
                        class EcgAnnotation ann;  //default annotation params
                        if (argc >= 3 + 1) {
                                wcscpy_s(params, _MAX_PATH, argv[3]);
                                parse_params(ann);
                        }                                                


                        wprintf(L" getting QRS complexes... ");
                        tic();
                        int** qrsAnn = ann.GetQRS(data, size, sr, L"filters");         //get QRS complexes                        
                        if (qrsAnn) {
                                wprintf(L" %d beats.\n", ann.GetQrsNumber());
                                ann.GetEctopics(qrsAnn, ann.GetQrsNumber(), sr);        //label Ectopic beats

                                wprintf(L" getting P, T waves... ");
                                int annNum = 0;
                                int** ANN = ann.GetPTU(data, size, sr, L"filters", qrsAnn, ann.GetQrsNumber());     //find P,T waves
                                if (ANN) {
                                        annNum = ann.GetEcgAnnotationSize();
                                        wprintf(L" done.\n");
                                        toc();
                                        wprintf(L"\n");
                                        //save ECG annotation
                                        wcscpy(annName, argv[1]);
                                        change_extension(annName, L".atr");
                                        ann.SaveAnnotation(annName, ANN, annNum);
                                } else {
                                        ANN = qrsAnn;
                                        annNum = 2 * ann.GetQrsNumber();
                                        wprintf(L" failed.\n");
                                        toc();
                                        wprintf(L"\n");
                                }
                                
                                //printing out annotation
                                for (int i = 0; i < annNum; i++) {
                                        int smpl = ANN[i][0];
                                        int type = ANN[i][1];

                                        msec = int(((double)smpl / sr) * 1000.0);
                                        signal.mSecToTime(msec, h, m, s, ms);

                                        wprintf(L"%10d %02d:%02d:%02d.%03d   %s\n", smpl, h, m, s, ms, anncodes[type]);
                                }

                                //saving RR seq
                                vector<double> rrs;
                                vector<int> rrsPos;

                                wcscpy(hrvName, argv[1]);
                                change_extension(hrvName, L".hrv");
                                if (ann.GetRRseq(ANN, annNum, sr, &rrs, &rrsPos)) {
                                        FILE *fp = _wfopen(hrvName, L"wt");
                                        for (int i = 0; i < (int)rrs.size(); i++)
                                                fwprintf(fp, L"%lf\n", rrs[i]);
                                        fclose(fp);

                                        wprintf(L"\n mean heart rate: %.2lf", signal.Mean(&rrs[0], (int)rrs.size()));
                                }

                        } else {
                                wprintf(L" could not get QRS complexes. make sure you have got \"filters\" directory in the ecg application dir.");
                                exit(1);
                        }

                } else {
                        wprintf(L" failed to read %s file", argv[1]);
                        exit(1);
                }

        }

        return 0;
}

void help()
{
        wprintf(L"usage: ecg.exe physionetfile.dat [LeadNumber] [params]\n");
        wprintf(L"       do not forget about \\filters dir to be present.");
}

static LARGE_INTEGER m_nFreq;
static LARGE_INTEGER m_nBeginTime;

void tic()
{
        QueryPerformanceFrequency(&m_nFreq);
        QueryPerformanceCounter(&m_nBeginTime);
}
void toc()
{
        LARGE_INTEGER nEndTime;
        __int64 nCalcTime;

        QueryPerformanceCounter(&nEndTime);
        nCalcTime = (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;

        wprintf(L" processing time: %d ms\n", nCalcTime);
}

void change_extension(wchar_t* path, const wchar_t* ext)
{
        for (int i = (int)wcslen(path) - 1; i > 0; i--) {
                if (path[i] == '.') {
                        path[i] = 0;
                        wcscat(path, ext);
                        return;
                }
        }
        wcscat(path, ext);
}

int parse_params(class EcgAnnotation &ann)
{
        FILE* fp = _wfopen(params, L"rt");
        if (fp != 0) {
                ANNHDR hdr;
                int res = 0;
                res = fwscanf(fp, L"%*s %d %*s %d"
                                  L"%*s %lf %*s %lf %*s %lf %*s %d %*s %lf"
                                  L"%*s %lf %*s %lf %*s %lf %*s %lf"
                                  L"%*s %lf %*s %lf %*s %d",  
                                   &hdr.minbpm, &hdr.maxbpm,
                                   &hdr.minQRS, &hdr.maxQRS, &hdr.qrsFreq, &hdr.ampQRS, &hdr.minUmV,
                                   &hdr.minPQ, &hdr.maxPQ, &hdr.minQT, &hdr.maxQT, 
                                   &hdr.pFreq, &hdr.tFreq, &hdr.biTwave);
                if (res == 14) {
                        PANNHDR phdr = ann.GetAnnotationHeader();
                        memcpy(phdr, &hdr, sizeof(ANNHDR));
                        wprintf(L" using annotation params from file %s\n", params);
                        wprintf(L"  minBpm  %d\n"  
                                L"  maxBpm  %d\n" 
                                L"  minQRS  %lg\n" 
                                L"  maxQRS  %lg\n" 
                                L" qrsFreq  %lg\n"
                                L"  ampQRS  %d\n"
                                L"  minUmV  %lg\n"
                                L"   minPQ  %lg\n"
                                L"   maxPQ  %lg\n" 
                                L"   minQT  %1f\n" 
                                L"   maxQT  %lg\n" 
                                L"   pFreq  %lg\n"  
                                L"   tFreq  %lg\n"
                                L" biTwave  %d\n\n", hdr.minbpm, hdr.maxbpm,
                                                   hdr.minQRS, hdr.maxQRS, hdr.qrsFreq, hdr.ampQRS, hdr.minUmV,
                                                   hdr.minPQ, hdr.maxPQ, hdr.minQT, hdr.maxQT, 
                                                   hdr.pFreq, hdr.tFreq, hdr.biTwave);
                        fclose(fp);
                        return 0;
                }
                else {
                        fclose(fp);
                        wprintf(L" failed to read %s annotation params file, using default ones instead.\n", params);
                        return res;                                
                }
        }
        else {
                wprintf(L" failed to open %s annotation params file, using default ones instead.\n", params);
                return -1;
        }
}