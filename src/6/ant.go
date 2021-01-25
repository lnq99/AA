package main

import (
	"math"
	"math/rand"
)

const (
	Alpha  = 1.8
	Beta   = 1.0
	Tau0   = 1.0
	Rho    = 0.5
	Q      = 20.0
	TauMin = 0.1
)

type Param struct {
}

type Ant struct {
	env       *Env
	visited   [][]int
	isVisited [][]bool
	pos       int
	route     []int
}

// graph + pheromone matrix + params
type Env struct {
	g            [][]int
	ph           [][]float64
	a, b, q, rho float64
}

func tspBrute(g [][]int) (int, []int) {
	n := len(g)
	min := math.MaxInt64
	rMin := make([]int, n)

	routes := [][]int{}
	r := []int{}
	getRoutes(0, g, r, &routes)

	for i := 0; i < len(routes); i++ {
		curRoute := routes[i]
		cur := calcLenRoute(g, curRoute)

		if cur < min {
			min = cur
			rMin = curRoute
		}
	}

	return min, rMin
}

func tspACO(e *Env, d int) (int, []int) {
	n := len(e.g)
	min := make([]int, n)
	rMin := make([][]int, n)

	for i := 0; i < d; i++ {
		for j := 0; j < n; j++ {
			a := e.createAnt(j)
			a.moveAnt()
			cur := calcLenRoute(e.g, a.route)

			if (cur < min[j]) || (min[j] == 0) {
				min[j] = cur
				rMin[j] = a.route
			}
		}
	}

	m := min[0]
	r := rMin[0]
	for i, v := range min {
		if v < m {
			m = v
			r = rMin[i]
		}
	}

	return m, r
}

func calcLenRoute(g [][]int, route []int) int {
	d := 0
	l := len(g) - 1

	for j := 0; j < l; j++ {
		d += g[route[j]][route[j+1]]
	}
	d += g[route[l]][route[0]]
	return d
}

func (e *Env) createAnt(pos int) *Ant {
	n := len(e.g)
	a := Ant{
		env:       e,
		visited:   make([][]int, n),
		isVisited: make([][]bool, n),
		pos:       pos,
		route:     []int{pos},
	}

	for i := 0; i < n; i++ {
		a.visited[i] = make([]int, n)
		for j := 0; j < len(e.g[i]); j++ {
			a.visited[i][j] = e.g[i][j]
		}
	}

	for i := range a.isVisited {
		a.isVisited[i] = make([]bool, n)
	}

	return &a
}

func createEnv(g [][]int) *Env {
	e := Env{
		g:   g,
		ph:  make([][]float64, len(g), len(g)),
		a:   Alpha,
		b:   Beta,
		q:   Q,
		rho: Rho,
	}

	initPheromone(e.ph)

	return &e
}

func initPheromone(ph [][]float64) {
	for i := 0; i < len(ph); i++ {
		ph[i] = make([]float64, len(ph))
		for j := range ph[i] {
			ph[i][j] = Tau0
		}
	}
}

func (a *Ant) moveAnt() {
	l := len(a.env.g) - 1
	for i := 0; i < l; i++ {
		prob := a.getProb()
		way := nextCity(prob)
		a.follow(way)
		a.route = append(a.route, way)
		if way == -1 {
			break
		}
		a.updatePheromone()
	}
}

func (a *Ant) getProb() []float64 {
	p := []float64{}
	var sum float64

	for i, l := range a.visited[a.pos] {
		if l != 0 {
			d := math.Pow((float64(1)/float64(l)), a.env.a) * math.Pow(a.env.ph[a.pos][i], a.env.b)
			p = append(p, d)
			sum += d
		} else {
			p = append(p, 0)
		}
	}

	for _, l := range p {
		l /= sum
	}

	return p
}

func (a *Ant) updatePheromone() {
	delta := 0.0

	for i := 0; i < len(a.env.ph); i++ {
		for j, ph := range a.env.ph[i] {
			if a.env.g[i][j] != 0 {
				if a.isVisited[i][j] {
					delta = a.env.q / float64(a.env.g[i][j])
				} else {
					delta = 0
				}
				a.env.ph[i][j] = (1 - a.env.rho) * (ph + delta)
			}

			if a.env.ph[i][j] <= 0 {
				a.env.ph[i][j] = TauMin
			}
		}
	}
}

func (a *Ant) follow(path int) {
	for i := range a.visited {
		a.visited[i][a.pos] = 0
	}
	a.isVisited[a.pos][path] = true
	a.pos = path
}

func nextCity(p []float64) int {
	var sum float64

	for _, j := range p {
		sum += j
	}

	r := rand.Float64() * sum
	sum = 0.0

	for i, v := range p {
		if r >= sum && r < sum+v {
			return i
		}
		sum += v
	}

	return len(p) - 1
}

func getRoutes(pos int, w [][]int, path []int, rts *[][]int) {
	path = append(path, pos)

	if len(path) < len(w) {
		for i := 0; i < len(w); i++ {
			if !isExist(path, i) {
				getRoutes(i, w, path, rts)
			}
		}
	} else {
		*rts = append(*rts, path)
	}
}

func isExist(a []int, v int) bool {
	for _, val := range a {
		if v == val {
			return true
		}
	}

	return false
}
