# require(matrixcalc)
require(lpSolve)

simplex.part <- function(constants, G, b, P, V)
{
  while(TRUE)
  {
    #if(is.singular.matrix(G[, P]))
    #{
    #   cat("Keskeytys.\n")
    #   return(NA)
    #}
    # Edellä oleva saattaa hidastaa suoritusta aika paljon suuremmissa systeemeissä,
    # joten sitä ei välttämättä tarvitse.
    xx <- solve(G[, P], b)
    
    ww <- t(solve(t(G[, P]), constants[P]))
    zz <- ww %*% G[, V] - constants[V]
    
    if(all(zz <= 0))
    {
      return(list(indeksit = P, kanta = xx))
    }
    
    k <- which.max(zz)
    
    y.k <- solve(G[, P], G[, V[k]])
    if(all(y.k <= 0))
    {
      cat("Tehtävä on rajoittamaton.\n")
      return(NA)
    }
    
    yy <- xx[y.k > 0]/y.k[y.k > 0]
    
    r <- which.min(yy)
    if(yy[r] == 0)
    {
      cat("Keskeytys.\n")
      return(NA)
    }
    
    xx <- c(xx[-r] - yy[r]*y.k[-r], yy[r])
    l <- P[r]
    P <- c(P[P != P[r]], V[k])
    
    V <- c(V[V != V[k]], l)
  }
}

auxiliary <- function(A, b)
{
  m <- dim(A)[1]
  n <- dim(A)[2]
  
  constants <- as.matrix(rep(1, m))
  constants <- rbind(as.matrix(rep(0, n)), constants)
  
  G <- A
  for(i in 1:m)
  {
    vv <- as.matrix(rep(0, m))
    vv[i] <- 1
    G <- cbind(G, vv)
  }
  
  P <- (n+1):(n+m)
  V <- 1:n
  
  sol <- simplex.part(constants, G, b, P, V)
  if(!is.list(sol))
  {
    return(NA)
  }
  else if(any(sol$indeksit %in% (n + 1):(n + m)))
  {
    cat("Keskeytys.\n")
    return(NA)
  }
  else
  {
    return(sol$indeksit)
  }
}

simplex <- function(const, A, b, d = 'eq')
{
  if(is.vector(const))
  {
    const <- as.matrix(const)
  }
  
  if(is.vector(b))
  {
    b <- as.matrix(b)
  }
  
  n <- length(const)
  m <- length(b)
  
  if(is.vector(A))
  {
    A <- matrix(A, nrow = m, ncol = n, byrow = TRUE)
  }
  
  if(!is.matrix(const) || !is.matrix(A) || !is.matrix(b))
  {
    cat("Keskeytys.\n")
    return(NA)
  }
  
  if(d == 'eq')
  {
    if(m > n)
    {
      cat("Keskeytys.\n")
      return(NA)
    }
    if(m == n)
    {
      if(is.singular.matrix(A))
      {
        cat("Keskeytys.\n")
        return(NA)
      }
      else
      {
        xx <- solve(A, b)
        if(all(xx >= 0))
        {
          return(xx)
        }
      }
    }
    
    A[b < 0, ] <- (-1) * A[b < 0, ]
    b[b < 0] <- (-1) * b[b < 0]
    sol <- auxiliary(A, b)
    if(!is.numeric(sol))
    {
      return(NA)
    }
    else
    {
      sol <- simplex.part(const, A, b, sol, (1:n)[-sol])
      if(!is.list(sol))
      {
        return(NA)
      }
      else
      {
        xx <- rep(0, n)
        xx[sol$indeksit] <- sol$kanta
        return(xx)
      }
    }
  }
  else if(d == 'leq')
  {
    const <- rbind(const, as.matrix(rep(0, m)))
    A <- cbind(A, diag(nrow = m))
    
    if(any(b < 0))
    {
      A[b < 0, ] <- (-1) * A[b < 0, ]
      b[b < 0] <- (-1) * b[b < 0]
      sol <- auxiliary(A, b)
      if(!is.numeric(sol))
      {
        return(NA)
      }
      else
      {
        sol <- simplex.part(const, A, b, sol, (1:(n + m))[-sol])
        if(!is.list(sol))
        {
          return(NA)
        }
        else
        {
          xx <- rep(0, n + m)
          xx[sol$indeksit] <- sol$kanta
          return(xx[1:n])
        }
      }
    }
    else
    {
      P <- (n+1):(n+m)
      V <- 1:n
      sol <- simplex.part(const, A, b, P, V)
      
      if(!is.list(sol))
      {
        return(NA)
      }
      else
      {
        xx <- rep(0, n + m)
        xx[sol$indeksit] <- sol$kanta
        return(xx[1:n])
      }
    }
  }
  else if(d == 'geq')
  {
    const <- rbind(const, as.matrix(rep(0, m)))
    
    A <- cbind(A, diag(rep(-1, m)))
    
    if(all(b < 0))
    {
      A <- (-1)*A
      b <- (-1)*b
      P <- (n+1):(n+m)
      V <- 1:n
      sol <- simplex.part(const, A, b, P, V)
      if(!is.list(sol))
      {
        return(NA)
      }
      else
      {
        xx <- rep(0, n + m)
        xx[sol$indeksit] <- sol$kanta
        return(xx[1:n])
      }
    }
    else
    {
      A[b < 0, ] <- (-1) * A[b < 0, ]
      b[b < 0] <- (-1) * b[b < 0]
      sol <- auxiliary(A, b)
      if(!is.numeric(sol))
      {
        return(NA)
      }
      else
      {
        sol <- simplex.part(const, A, b, sol, (1:(n + m))[-sol]) # Muista käsitellä NA
        if(!is.list(sol))
        {
          return(NA)
        }
        else
        {
          xx <- rep(0, n + m)
          xx[sol$indeksit] <- sol$kanta
          return(xx[1:n])
        }
      }
    }
  }
  else if(!is.character(d))
  {
    if(!all(d %in% c(-1, 0, 1)))
    {
      cat("Keskeytys.\n")
      return(NA)
    }
    
    j <- length(d[d == -1 || d == 1])
    
    A <- cbind(A, diag(d)[, d == -1 || d == 1])
    
    if(m > n + j)
    {
      cat("Keskeytys.\n")
      return(NA)
    }
    
    if(m == n + j)
    {
      if(is.singular.matrix(A))
      {
        cat("Keskeytys.\n")
        return(NA)
      }
      else
      {
        xx <- solve(A, b)
        if(all(xx >= 0))
        {
          return(xx[1:n])
        }
      }
    }
    
    const <- rbind(const, as.matrix(rep(0, j)))
    A[b < 0, ] <- (-1) * A[b < 0, ]
    b[b < 0] <- (-1) * b[b < 0]
    
    sol <- auxiliary(A, b)
    if(!is.numeric(sol))
    {
      return(NA)
    }
    else
    {
      sol <- simplex.part(const, A, b, sol, (1:(n + j))[-sol])
      if(!is.list(sol))
      {
        return(NA)
      }
      else
      {
        xx <- rep(0, n + j)
        xx[sol$indeksit] <- sol$kanta
        return(xx[1:n])
      }
    }
  }
}

# Oman simplex-funktion vertailu valmiiseen lineaariseen optimointiin liittyvään funktioon
AA <- matrix(c(3, 1, 2, 1, -3, 1, 1, 2, 3, 4, 3, -1), nrow = 4, ncol = 3, byrow = TRUE)

start.time <- Sys.time()
simplex(as.matrix(c(12, 7, 10)), AA, as.matrix(c(2, 4, 3, 1)), 'geq')
end.time <- Sys.time()
end.time - start.time

start.time <- Sys.time()
lp("min", as.matrix(c(12, 7, 10)), AA, c(">=", ">=", ">=", ">="), as.matrix(c(2, 4, 3, 1)))$solution
end.time <- Sys.time()
end.time - start.time

# Rajoittamattoman ongelman tapaus
simplex(c(-1, 4), c(-2, 1, -1, -2), c(-1, -2), 'leq')

#vv <- as.matrix(runif(10, 1, 2))
#uu <- as.matrix(runif(10, -1, 1))
#GG <- matrix(runif(200, -1, 1), nrow = 20)
#GG[(GG %*% vv) > 0, ] <- (-1) * GG[(GG %*% vv) > 0]
#bb <- as.matrix(runif(20, 0, 1)) + GG %*% uu
#cc <- runif(10, -2, -1)
#simplex(cc, GG, bb, 'leq')