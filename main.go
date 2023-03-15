package main

import (
	"fmt"
	"net"
	"sort"
)

////并发的tcp扫描器：
////1 非并发，循环耗时相对较长。
// func main() {
//      //端口扫描
//      for i := 21; i < 150; i++ {
//              address := fmt.Sprintf("47.96.38.84:%d", i)
//              conn, err := net.Dial("tcp", address)
//              if err != nil {
//                      fmt.Printf("%v 关闭了\n", address)
//                      continue
//              }
//              conn.Close()
//              fmt.Printf("%v 打开了\n", address)
//      }
// }

// //并发的tcp扫描器：
// //2 协程go关键字
// func main() {
// 	start := time.Now()   //看一下耗时
// 	var wg sync.WaitGroup //
// 	for i := 21; i < 150; i++ {
// 		wg.Add(1)
// 		go func(j int) {
// 			defer wg.Done()
// 			address := fmt.Sprintf("47.96.38.84:%d", j)
// 			conn, err := net.Dial("tcp", address)
// 			if err != nil {
// 				fmt.Printf("%v 端口关闭\n", address)
// 				return
// 			}
// 			conn.Close()
// 			fmt.Printf("%v 端口打开\n", address)
// 		}(i)
// 	}
// 	wg.Wait()
// 	times := time.Since(start) / 1e9
// 	fmt.Printf("\n\n扫描耗时：%v\n", times)
// }

////并发的tcp扫描器：
////3 goroutine

func Worker(ports chan int, results chan int) {
	for p := range ports {
		address := fmt.Sprintf("47.96.38.84:%d", p)
		conn, err := net.Dial("tcp", address)
		if err != nil {
			//fmt.Printf("%v 关闭了\n", address)
			results <- 0
			continue
		}
		conn.Close()
		//fmt.Printf("%v 打开了\n", address)
		results <- p
	}

}

const PORT_NUM = 10000 //扫描端口总数量

func main() {
	ports := make(chan int, 1000)
	results := make(chan int)
	//将端口分类展示
	var openports []int
	var closeports []int
	//var wg sync.WaitGroup
	for i := 0; i < cap(ports); i++ {
		go Worker(ports, results)
	}
	//并发执行
	go func() {
		for i := 1; i < PORT_NUM; i++ {
			ports <- i
		}
	}()

	
	for i := 1; i < PORT_NUM; i++ {
		port := <-results
		if port != 0 {
			openports = append(openports, port)
		} else {
			closeports = append(closeports, port)
		}
	}

	close(ports)
	close(results)

	sort.Ints(openports)
	sort.Ints(closeports)

	// for _, port := range closeports {
	// 	fmt.Printf("%d 端口关闭\n", port)
	// }
	//
	for _, port := range openports {
		fmt.Printf("%d 端口打开\n", port)
	}

}

	
