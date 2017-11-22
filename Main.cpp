#include "mpi.h"
#include <stdio.h>

#define N 100 // Jumlah Data

int main(int argc, char *argv[])
{
	int myrank, size; 
	int A[N], B[N], i, x, mulai, selesai, hasil = 0, total = 0, akhir = 0;
	MPI_Status status;

	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Variable untuk menampung berapa banyak yang harus dikerjakan oleh masing2 pemeroses */
	x = N / size; // x = N dibagi size, artinya Jumlah semua data (N) dibagi jumlah pemeroses (size).
	/*Kalau N = 100 dan size 4 jadi, 100/4 berarti 1 pemeroses mengerjakan 25 operasi. nah 25 operasi disimpan ke variable x */

	/*Rules Untuk Mencari nilai mulai dan berhenti saat melakukan perulangan*/
	mulai = myrank * x; /* Pemeroses dikali x tadi. misal kalau pemeroses ada diposisi 1, berarti 1*25, jadi i dimulai 25 */
	selesai = mulai + x; /* Nilai mulai tadi ditambah dengan x. misal nilai mulai tadi 25. jadi nilai 25 ditambah 25 jadi perulangan berhenti di 50 */

	if (myrank == 0) {
		for (i = 0; i<N; i++) { /* lakukan Perulangan sebanyak jumlah data */
								/* buat data secara random (range 0 sampai N) dan kita simpan kedalam array A dan B untuk data penjumlahan */
			A[i] = (rand() % 100);  // rand() itu fungsi yang disediain oleh C untuk membuat angka random. rand()%N artinya buat angka random dari 0 sampai N.
			B[i] = (rand() % 100);

			//printf("Nilai yang harus dihitung ke-%d : %d + %d = ?", i);
		}
	}

	/* Broadcast data A dan B dari pemeroses 0 tadi*/
	MPI_Bcast(A, N, MPI_INT, 0, MPI_COMM_WORLD); // Kirim data A, sebanyak N, dengan type data INT, dari pemeroses 0.
	MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD); // Kirim data B, sebanyak N, dengan type data INT, dari pemeroses 0.

	if (myrank == 0) { // pemeroses 0
		printf("\nData pada P-%d", myrank);
		for (i = mulai; i < selesai; i++) {
			hasil = A[i] + B[i];
			printf("\nData ke-%d = %d + %d = %d", i, A[i], B[i], hasil);
			total += hasil;
		}
		printf("\nTotal penjumlahan pada P-%d adalah %d \n", myrank, total);

		n4 = total;

		printf("\nData dari P-0 : %d.", n4);
		MPI_Recv(&n1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		printf("\nData dari P-1 : %d, Sudah diterima.\n", n1);
		MPI_Recv(&n2, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
		printf("Data dari P-2 : %d, Sudah diterima.\n", n2);
		MPI_Recv(&n3, 1, MPI_INT, 3, 2, MPI_COMM_WORLD, &status);
		printf("Data dari P-3 : %d, Sudah diterima.\n", n3);

		akhir = n1 + n2 + n3 + n4;
		printf("\nHasil Akhir : %d", akhir);
	}
	if (myrank == 1) { // pemeroses 1
		printf("\nData pada P-%d", myrank); // Data pada P-1
		for (i = mulai; i < selesai; i++) { // 25 sampai 49
			hasil = A[i] + B[i]; // hasil = nilai A ke i, ditambah nilai b ke i
			printf("\nData ke-%d = %d + %d = %d", i, A[i], B[i], hasil);
			total += hasil; // Hitung semua nilai dari hasil yang sudah dikerjakan dan simpan ke variable total.
		}
		printf("\nTotal penjumlahan pada P-%d adalah %d \n", myrank, total);

		n1 = total; // ini variable untuk menyimpan dan meneruskan total penjumlahan yang dikerjakan P-1 
		MPI_Send(&n1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // kirim variable n1, yang terdiri dari 1 data, dengan type data int, kirim ke P-0, tag 0.
	}
	if (myrank == 2) { // pemeroses 2
		printf("\nData pada P-%d", myrank);
		for (i = mulai; i < selesai; i++) {
			hasil = A[i] + B[i];
			printf("\nData ke-%d = %d + %d = %d", i, A[i], B[i], hasil);
			total += hasil;
		}
		printf("\nTotal penjumlahan pada P-%d adalah %d \n", myrank, total);

		n2 = total;
		MPI_Send(&n2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); // Kirim Ke rank 0
	}
	if (myrank == 3) { // pemeroses 3
		printf("\nData pada P-%d", myrank);
		for (i = mulai; i < selesai; i++) {
			hasil = A[i] + B[i];
			printf("\nData ke-%d = %d + %d = %d", i, A[i], B[i], hasil);
			total += hasil;
		}
		printf("\nTotal penjumlahan pada P-%d adalah %d \n", myrank, total);

		n3 = total;
		MPI_Send(&n3, 1, MPI_INT, 0, 2, MPI_COMM_WORLD); // Kirim Ke rank 0
	}


	MPI_Finalize();
}